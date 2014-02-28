#include <iostream>
#include <thread>
#include <mutex>

#include "sniffer.hh"
#include "options.hh"
#include "logger.hh"

Sniffer*					Sniffer::_singleton = NULL;
std::map<std::string, int>                      Sniffer::_traffic;
std::map<std::string, int>                      Sniffer::_average;
std::set<std::string>                           Sniffer::_alert;
std::mutex                                      mtxTraffic;
std::mutex                                      mtxAlert;


Sniffer::Sniffer ()
{
}

Sniffer::~Sniffer ()
{
}

Sniffer*
Sniffer::getSniffer ()
{
  if (!Sniffer::_singleton)
    Sniffer::_singleton = new Sniffer ();

  return Sniffer::_singleton;
}

void
Sniffer::checkTraffic ()
{
  std::chrono::system_clock::time_point         start;
  std::chrono::system_clock::time_point         end;
  std::map<std::string, int>::iterator          it;
  int                                           max = 0;
  std::string                                   str;
  int                                           diff;

  while (true)
  {
    start = std::chrono::system_clock::now ();
    max = 0;
    str = "";

    mtxTraffic.lock ();

    for (it = _traffic.begin (); it != _traffic.end (); it++)
      if (it->second > max)
      {
        max = it->second;
        str = it->first;
      }

    Logger::log (Logger::DEBUG, "In thread traffic, diff time = " +
                 std::to_string (diff));
    Logger::log (Logger::TRAFFIC, "best hit: " + str + " HITS: " +
                 std::to_string (max));

    _traffic.clear ();

    mtxTraffic.unlock ();

    end = std::chrono::system_clock::now ();

    diff = difftime (std::chrono::system_clock::to_time_t (end),
                     std::chrono::system_clock::to_time_t (start));

    std::this_thread::sleep_for (std::chrono::seconds (10 - diff));
  }
}

void
Sniffer::checkAlert ()
{
  std::chrono::system_clock::time_point         start;
  std::chrono::system_clock::time_point         end;
  std::map<std::string, int>::iterator          it;
  std::set<std::string>::iterator               itt;
  int                                           diff;

  while (true)
  {
    start = std::chrono::system_clock::now ();

    mtxAlert.lock ();

    for (itt = _alert.begin (); itt != _alert.end (); )
      if (_average.find (*itt) == _average.end () ||
          _average.find (*itt)->second <= ALERT_THRESHOLD)
      {
        Logger::log (Logger::ALERT, "ALERT: " + *itt +
                     " ACCESS DECREASED UNDER THRESHOLD");
        _alert.erase (itt++);
      }
      else
        itt++;

    for (it = _average.begin (); it != _average.end (); it++)
      if (it->second > ALERT_THRESHOLD &&
          _alert.find (it->first) == _alert.end ())
      {
        Logger::log (Logger::ALERT, "ALERT: " + it->first +
                     " EXCEED THRESHOLD ACCESS: " +
                     std::to_string (it->second));
        Sniffer::_alert.insert (it->first);
      }

    Logger::log (Logger::DEBUG, "In thread Alert, diff time = " +
                 std::to_string (diff));

    _average.clear ();

    mtxAlert.unlock ();

    end = std::chrono::system_clock::now ();

    diff = difftime (std::chrono::system_clock::to_time_t (end),
                     std::chrono::system_clock::to_time_t (start));

    std::this_thread::sleep_for (std::chrono::seconds (120 - diff));
  }
}

void
count (std::string const&			str)
{
  mtxTraffic.lock ();

  if (Sniffer::_traffic.find (str) == Sniffer::_traffic.end ())
    Sniffer::_traffic[str] = 1;
  else
    Sniffer::_traffic[str]++;

  mtxTraffic.unlock ();
  mtxAlert.lock ();

  if (Sniffer::_average.find (str) == Sniffer::_average.end ())
    Sniffer::_average[str] = 1;
  else
    Sniffer::_average[str]++;

  mtxAlert.unlock ();

  Logger::log (Logger::DEBUG, std::to_string(Sniffer::_traffic[str]));
}

void
parseHTTP (const u_char*                        payload,
		    int				size)
{
  char						method[] = "GET";
  std::string					str;

  if (payload[0] != method[0] ||
      payload[1] != method[1] ||
      payload[2] != method[2])
    return;

  for (int i = 4; i < size; i++)
  {
    if (payload[i] == ' ' || (payload[i] == '/' && i > 4))
      break;
    else
      str.append (1, payload[i]);
  }

  Logger::log (Logger::DEBUG, str);
  count (str);
}

void
got_packet(u_char*				args,
	   const struct pcap_pkthdr*		header,
	   const u_char*			packet)
{
  const struct sniff_ethernet*                  ethernet;
  const struct sniff_ip*                        ip;
  const struct sniff_tcp*                       tcp;
  const u_char*                                 payload;
  u_int                                         size_ip;
  u_int                                         size_tcp;
  int                                           size_payload;

  ethernet = (struct sniff_ethernet*)(packet);
  ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
  size_ip = IP_HL(ip) * 4;
  if (size_ip < 20)
  {
    Logger::log (Logger::ERROR, "Invalid IP header length: " +
                 std::to_string (size_ip));
    return;
  }

  tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
  size_tcp = TH_OFF(tcp)*4;
  if (size_tcp < 20)
  {
    Logger::log (Logger::ERROR, "Invalid TCP header length: " +
                 std::to_string (size_tcp));
    return;
  }

  payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
  size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);

  if (size_payload > 0)
    parseHTTP (payload, size_payload);
}

int
Sniffer::snif ()
{
  std::thread                                   traffic (Sniffer::checkTraffic);
  std::thread                                   alert (Sniffer::checkAlert);

  pcap_loop (_handle, -1, got_packet, NULL);

  return ERR_NONE;
}

int
Sniffer::init ()
{
  char						errbuf[PCAP_ERRBUF_SIZE];
  struct bpf_program				fp;
  bpf_u_int32					mask;
  bpf_u_int32					net;

  if (pcap_lookupnet (Options::_dev, &net, &mask, errbuf) == -1)
  {
    Logger::log (Logger::ERROR, "Can't get netmask for device");
    net = 0;
    mask = 0;
  }

  _handle = pcap_open_live(Options::_dev, 1518, 1, 100, errbuf);
  if (_handle == NULL)
  {
    Logger::log (Logger::ERROR, "Couldn't open device: ", errbuf);
    return ERR_SNF;
  }

  if (pcap_datalink (_handle) != DLT_EN10MB)
  {
    Logger::log (Logger::ERROR, "Device doesn't provide ethernet headers");
    return ERR_SNF;
  }

  if (pcap_compile (_handle, &fp, FILTER, 0, net) == -1)
  {
    Logger::log (Logger::ERROR, "Couldn't parse filter: ", FILTER);
    Logger::log (Logger::DEBUG, pcap_geterr (_handle));
    return ERR_SNF;
  }

  if (pcap_setfilter (_handle, &fp) == -1)
  {
    Logger::log (Logger::ERROR, "Couldn't install filter: ", FILTER);
    Logger::log (Logger::DEBUG, pcap_geterr (_handle));
    return ERR_SNF;
  }

  return(ERR_NONE);
}
