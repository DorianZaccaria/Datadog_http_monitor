#include "main.hh"

int				main (int			argc,
				      char**			argv)
{
  OptCode			code;
  Options*			Opt = Options::getOpt ();

  Logger::init ();
  code = Opt->parse (argc, argv);

  switch (code)
  {
    case ERR_NONE:
      Logger::log (Logger::INFO, "Options parsed");

      Logger::log (Logger::INFO, "Program launched");
      return code;
    case ERR_ARG:
      Logger::log (Logger::ERROR, "Unrecognized argument");
      return code;
    case ERR_ERROR:
      Logger::log (Logger::ERROR, "Unexpected error");
      return code;
    default:
      Logger::log (Logger::ERROR, "Unexpected error");
      return code;
  }



  Logger::log (Logger::INFO, "Program terminated");
}
