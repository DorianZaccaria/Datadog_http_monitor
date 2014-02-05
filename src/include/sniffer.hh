#ifndef SNIFFER_HH_
# define SNIFFER_HH_

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
     * @brief the device na;e to use
     */
    char*				_dev = NULL;

};

#endif /* !SNIFFER_HH_ */
