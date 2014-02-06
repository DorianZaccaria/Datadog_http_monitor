#ifndef SNIFFER_HH_
# define SNIFFER_HH_

# include <pcap.h>
# include <netinet/in.h>
# include <map>
# include <string>

# define DEV "lo"
# define FILTER "port 80"

/*!
 * @brief ETHER_ADDR_LEN
 * the ethernet address length
 */
# define ETHER_ADDR_LEN 6
/*!
 * @brief SIZE_ETHERNET
 * the ethernet header length
 */
#define SIZE_ETHERNET 14


/*!
 * @struct sniff_ethernet
 * Ethernet header. For more information http://www.tcpdump.org/pcap.html
 */
struct sniff_ethernet {
    u_char ether_dhost[ETHER_ADDR_LEN];/* Destination host address */
    u_char ether_shost[ETHER_ADDR_LEN];/* Source host address */
    u_short ether_type;/* IP? ARP? RARP? etc */
};

/*!
 * @struct sniff_ip
 * IP header. For more information http://www.tcpdump.org/pcap.html
 */
struct sniff_ip {
    u_char ip_vhl;/* version << 4 | header length >> 2 */
    u_char ip_tos;/* type of service */
    u_short ip_len;/* total length */
    u_short ip_id;/* identification */
    u_short ip_off;/* fragment offset field */
#define IP_RF 0x8000/* reserved fragment flag */
#define IP_DF 0x4000/* dont fragment flag */
#define IP_MF 0x2000/* more fragments flag */
#define IP_OFFMASK 0x1fff/* mask for fragmenting bits */
    u_char ip_ttl;/* time to live */
    u_char ip_p;/* protocol */
    u_short ip_sum;/* checksum */
    struct in_addr ip_src,ip_dst; /* source and dest address */
};
#define IP_HL(ip)(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)(((ip)->ip_vhl) >> 4)

typedef u_int tcp_seq;

/*!
 * @struct sniff_tcp
 * TCP header. For more information http://www.tcpdump.org/pcap.html
 */
struct sniff_tcp {
    u_short th_sport;/* source port */
    u_short th_dport;/* destination port */
    tcp_seq th_seq;/* sequence number */
    tcp_seq th_ack;/* acknowledgement number */

    u_char th_offx2;/* data offset, rsvd */
#define TH_OFF(th)(((th)->th_offx2 & 0xf0) >> 4)
    u_char th_flags;
    #define TH_FIN 0x01
    #define TH_SYN 0x02
    #define TH_RST 0x04
    #define TH_PUSH 0x08
    #define TH_ACK 0x10
    #define TH_URG 0x20
    #define TH_ECE 0x40
    #define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win;/* window */
    u_short th_sum;/* checksum */
    u_short th_urp;/* urgent pointer */
};

/*!
 * @class Sniffer
 * This class is the core of the program, it captures http packets and
 * calculates averages access.
 */
class Sniffer
{
  public:
    /*!
     * @brief getSniffer
     * initializes the singleton instance
     */
    static Sniffer*			getSniffer ();
    /*!
     * @brief destructor
     */
    ~Sniffer ();
    /*!
     * @brief snif
     * starts the packet sniffing
     */
    int					snif ();
    /*!
     * @brief init
     * initializes the ethernet interface
     */
    int					init ();
    /*!
     * @brief the counter for the http access
     */
    static std::map<std::string, int>	_map;

  private:
    /*!
     * @brief constructor
     */
    Sniffer ();

    /*!
     * @brief the unique instance of this class
     */
    static Sniffer*			_singleton;
    /*!
     * @brief the device name to use
     */
//    char				_dev = "l0";

    /*!
     * @brief defines the sniffing session
     */
    pcap_t*				_handle = NULL;

};

/*!
 * @brief got_packet
 * the handler when a packet is received
 */
void				got_packet(u_char*,
					   const struct pcap_pkthdr*,
					   const u_char*);
    /*!
     * @brief if the http request is a GET, extract the file name and
     * count it
     */
//    void				parseHTTP (const u_char*,
//						   int);
    /*!
     * @brief if the http request is a GET, extract the file name and
     * count it
     */
//  void				count (std::string const&);

#endif /* !SNIFFER_HH_ */
