#include "sniffer.hh"
#include "options.hh"
#include "logger.hh"

Sniffer*			Sniffer::_singleton = NULL;

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

int
Sniffer::snif ()
{
  struct pcap_pkthdr		header;
  const u_char*			packet;

  packet = pcap_next(_handle, &header);
  printf("Jacked a packet with length of [%d]\n", header.len);
  printf("Jacked a packet: [%s]\n", packet);
  pcap_close(_handle);

  return ERR_NONE;
}

int
Sniffer::init ()
{
  char				errbuf[PCAP_ERRBUF_SIZE];
  struct bpf_program		fp;
  bpf_u_int32			mask;
  bpf_u_int32			net;
  char dev[] = "wlan1";
  char filter_exp[] = "port 80";
/*
  _dev = pcap_lookupdev (errbuf);
  if (_dev == NULL)
  {
    Logger::log (Logger::ERROR, "Couldn't find device: ", errbuf);
    return ERR_SNF;
  }
*/
  if (pcap_lookupnet (dev, &net, &mask, errbuf) == -1)
  {
    Logger::log (Logger::ERROR, "Can't get netmask for device");
    net = 0;
    mask = 0;
  }

  _handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
  if (_handle == NULL)
  {
    Logger::log (Logger::ERROR, "Couldn't open device: ", errbuf);
    return ERR_SNF;
  }

  if (pcap_datalink (_handle) != DLT_EN10MB)
  {
    Logger::log (Logger::ERROR, "Device Doesn't provide Ethernet headers");
    return ERR_SNF;
  }

  if (pcap_compile (_handle, &fp, filter_exp, 0, net) == -1)
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
