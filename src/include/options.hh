#ifndef OPTIONS_HH_
# define OPTIONS_HH_

/*!
 * @enum OptCode
 * defines some return values
 */
typedef enum OptCode
{
  ERR_NONE = 0,
  ERR_ARG,
  ERR_ERROR
} OptCode;

/*!
 * @class Options
 * This class is a singleton which contain runtime parameters
 */
class Options
{
  public:
    /*!
     * @brief the debug option variable, in public in order to easily
     * access to it.
     */
    static bool				_debug;
    /*!
     * @brief the alert option variable, in public in order to easily
     * access to it.
     */
    static bool				_alert;
    /*!
     * @brief the trafic option variable, in public in order to easily
     * access to it.
     */
    static bool				_traffic;

    /*!
     * @brief getOpt
     * returns the Options instance and initilizes it if necessary
     * @return Options, the unique instance of this class
     */
    static Options*			getOpt ();

    /*!
     * @brief destructor
     */
    ~Options ();

    /*!
     * @brief parse
     * parses all options given in argument to the program
     *
     * @param const int: the number of argument
     * @param const char**: the arguments array
     */
    OptCode				parse (int const,
					       char**);


  private:
    /*!
     * @brief constructor
     */
    Options ();
    /*!
     * @brief the Options instance
     */
    static Options*			_singleton;
};

#endif /* !OPTIONS_HH_ */
