#include <iostream>


#include "sniffer.hh"
#include "options.hh"
#include "logger.hh"

Sniffer*					Sniffer::_singleton = NULL;
std::map<std::string, int>	Sniffer::_map;

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
print_hex_ascii_line(const u_char *payload, int len, int offset)
{

  int i;
  int gap;
  const u_char *ch;

  /* offset */
  printf("%05d   ", offset);

  /* hex */
  ch = payload;
  for(i = 0; i < len; i++) {
    printf("%02x ", *ch);
    ch++;
    /* print extra space after 8th byte for visual aid */
    if (i == 7)
      printf(" ");
  }
  /* print space to handle line less than 8 bytes */
  if (len < 8)
    printf(" ");

  /* fill hex gap with spaces if not full line */
  if (len < 16) {
    gap = 16 - len;
    for (i = 0; i < gap; i++) {
      printf("   ");
    }
  }
  printf("   ");

  /* ascii (if printable) */
  ch = payload;
  for(i = 0; i < len; i++) {
    if (isprint(*ch))
      printf("%c", *ch);
    else
      printf(".");
    ch++;
  }

  printf("\n");

  return;
}

/*
 * print packet payload data (avoid printing binary data)
 */
void
print_payload(const u_char *payload, int len)
{

  int len_rem = len;
  int line_width = 16;   /* number of bytes per line */
  int line_len;
  int offset = 0;     /* zero-based offset counter */
  const u_char *ch = payload;

  if (len <= 0)
    return;

  /* data fits on one line */
  if (len <= line_width) {
    print_hex_ascii_line(ch, len, offset);
    return;
  }

  /* data spans multiple lines */
  for ( ;; ) {
    /* compute current line length */
    line_len = line_width % len_rem;
    /* print line */
    print_hex_ascii_line(ch, line_len, offset);
    /* compute total remaining */
    len_rem = len_rem - line_len;
    /* shift pointer to remaining bytes to print */
    ch = ch + line_len;
    /* add offset */
    offset = offset + line_width;
    /* check if we have line width chars or less */
    if (len_rem <= line_width) {
      /* print last line and get out */
      print_hex_ascii_line(ch, len_rem, offset);
      break;
    }
  }

  return;
}

void
count (std::string const&			str)
{
  if (Sniffer::_map.find (str) == Sniffer::_map.end ())
    Sniffer::_map[str] = 1;
  else
    Sniffer::_map[str]++;

  Logger::log (Logger::DEBUG, str);
  std::cout << Sniffer::_map[str] << std::endl;
}

void
parseHTTP (const u_char*		payload,
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
    if (payload[i] == ' ')
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
  const struct sniff_ethernet *ethernet; /* The ethernet header */
  const struct sniff_ip *ip; /* The IP header */
  const struct sniff_tcp *tcp; /* The TCP header */
  const u_char *payload; /* Packet payload */

  u_int size_ip;
  u_int size_tcp;
  int size_payload;

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

  size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);
  //Logger::log (Logger::INFO, "toto");
  ethernet = ethernet;

  if (size_payload > 0) {
//    printf("   Payload (%d bytes):\n", size_payload);
    //print_payload(payload, size_payload);
    parseHTTP (payload, size_payload);
  }
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
