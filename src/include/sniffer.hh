#ifndef SNIFFER_HH_
# define SNIFFER_HH_

# include <pcap.h>
# include <netinet/in.h>
# include <map>
# include <set>
# include <string>
# include <ctime>
# include <chrono>

# include "networking_structure.hh"

# define FILTER "port 80"
# define ALERT_THRESHOLD 3

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
     * @brief checkAlert
     * every 10 second this function print a messsage with the most loaded part of the website
     * This function is launch as a thread.
     */
    static void                         checkTraffic ();
    /*!
     * @brief checkAlert
     * check if an alert should be raised based on the average map
     * This function is launch as a thread.
     */
    static void                         checkAlert ();
    /*!
     * @brief init
     * initializes the ethernet interface
     */
    int					init ();
    /*!
     * @brief the counter for the http access
     */
    static std::map<std::string, int>	_traffic;
    /*!
     * @brief This map is used to calculated the average access for 120 seconds
     */
    static std::map<std::string, int>	_average;
    /*!
     * @brief This set stocks the folder in alert state
     */
    static std::set<std::string>        _alert;

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
     * @brief defines the sniffing session
     */
    pcap_t*				_handle = NULL;

};

/*!
 * @brief got_packet
 * This function is the callback handled when a packet is received
 */
void				got_packet(u_char*,
					   const struct pcap_pkthdr*,
					   const u_char*);
/*!
 * @brief if the http request is a GET, this function extracts the file name and
 * calls the count function
 */
void				parseHTTP (const u_char*,
                                           int);
/*!
 * @brief if the request concer a new folder this function add an entry in _map
 * with the value 1. Otherwise it increments the key's value (the folder name)
 */
void				count (std::string const&);

#endif /* !SNIFFER_HH_ */
