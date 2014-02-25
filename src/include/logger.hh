#ifndef LOGGER_HH_
# define LOGGER_HH_

# include <string>

/*!
 * @class Logger
 * This class is the main tool to display a message
 */
class Logger
{
  public:
    /*!
     * @enum category
     * defies the log message category
     */
    typedef enum Category
    {
      ERROR = 0,
      INFO,
      TRAFFIC,
      ALERT,
      DEBUG
    } Category;

    /*!
     * @brief init
     * initializes the singleton instance
     */
    static void				init ();
    /*!
     * @brief destructor
     */
    ~Logger ();
    /*!
     * @brief log
     * This function gives the main interface to print a message on the
     * standard output
     *
     * @param Category: the message category
     * @param std::string: the message
     */
    static void				log (Category const,
					     std::string const&);
    /*!
     * @brief log
     * This function gives the main interface to print a message on the
     * standard output
     *
     * @param Category: the message category
     * @param std::string: the message
     * @param char*: extra message
     */
    static void				log (Category const,
					     std::string const&,
					     char const*);

  private:
    /*!
     * @brief constructor
     */
    Logger ();
    /*!
     * @brief the unique instance of this class
     */
    static Logger*			_singleton;
};

#endif /* !LOGGER_HH_ */
