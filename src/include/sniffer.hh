#ifndef SNIFFER_HH_
# define SNIFFER_HH_

# include <pcap.h>

# define DEV "wlan1"
# define FILTER "port 80"

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

#endif /* !SNIFFER_HH_ */
