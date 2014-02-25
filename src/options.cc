#include <iostream>
#include <getopt.h>

#include "options.hh"

Options*				Options::_singleton = NULL;
bool					Options::_debug = false;
bool					Options::_alert = true;
bool					Options::_traffic = true;
char*                                   Options::_dev;

Options::Options ()
{
}

Options::~Options ()
{
}

Options*
Options::getOpt ()
{
  if (!Options::_singleton)
    Options::_singleton = new Options ();
  return Options::_singleton;
}

OptCode
Options::parse (int const		argc,
		char**			argv)
{
  const struct option			longopts[] =
  {
    {"without-alert", no_argument, 0, 'a'},
    {"without-traffic", no_argument, 0, 't'},
    {"debug", no_argument, 0, 'd'},
    {"interface", required_argument, 0, 'i'},
    {0, 0, 0, 0}
  };
  int					index;
  int					iarg = 0;

  opterr = 1;

  while (true)
  {
    iarg = getopt_long (argc, argv, "atd:i", longopts, &index);

    if (iarg == -1)
      break;

    switch (iarg)
    {
      case 'a':
	Options::_alert = false;
	break;
      case 't':
	Options::_traffic = false;
	break;
      case 'd':
	Options::_debug = true;
	break;
      case 'i':
	Options::_dev = optarg;
	break;
      case '?':
	return ERR_ARG;
      default:
	return ERR_ERROR;
    }
  }

  if (_dev == '\0')
    return ERR_ARG;

  return ERR_NONE;
}
