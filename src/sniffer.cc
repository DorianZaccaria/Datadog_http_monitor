#include "sniffer.hh"
#include "options.hh"
#include "logger.hh"

Sniffer*					Sniffer::_singleton = NULL;

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

void got_packet(u_char*				args,
		const struct pcap_pkthdr*	header,
		const u_char*			packet)
{
  const struct sniff_ethernet *ethernet; /* The ethernet header */
  const struct sniff_ip *ip; /* The IP header */
  const struct sniff_tcp *tcp; /* The TCP header */
  const u_char *payload; /* Packet payload */

  u_int size_ip;
  u_int size_tcp;

  args = args;
  header = header;

  ethernet = (struct sniff_ethernet*)(packet);
  ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
  size_ip = IP_HL(ip)*4;
  if (size_ip < 20) {
    printf("   * Invalid IP header length: %u bytes\n", size_ip);
    return;
  }
  tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
  size_tcp = TH_OFF(tcp)*4;
  if (size_tcp < 20) {
    printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
    return;
  }
  payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
//  std::string str (payload);
  Logger::log (Logger::INFO, "toto");
  ethernet = ethernet;
  payload = payload;

}

int
Sniffer::snif ()
{
  struct pcap_pkthdr				header;
  //const u_char*					packet;

  pcap_loop (_handle, -1, got_packet, NULL);
  printf("Jacked a packet with length of [%d]\n", header.len);


  pcap_close(_handle);

  return ERR_NONE;
}

int
Sniffer::init ()
{
  char						errbuf[PCAP_ERRBUF_SIZE];
  struct bpf_program				fp;
  bpf_u_int32					mask;
  bpf_u_int32					net;
  char						dev[] = "lo";
  char						filter_exp[] = "port 80";
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

  _handle = pcap_open_live(dev, 1518, 1, 100, errbuf);
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
