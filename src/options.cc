#include <iostream>
#include <getopt.h>

#include "options.hh"

Options*				Options::_singleton = NULL;
bool					Options::_debug = false;
bool					Options::_alert = false;
bool					Options::_traffic = false;

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
    {"alert-only", no_argument, 0, 'a'},
    {"trafic-only", no_argument, 0, 't'},
    {"debug", no_argument, 0, 'd'},
    {0, 0, 0, 0}
  };
  int					index;
  int					iarg = 0;

  opterr = 1;

  while (true)
  {
    iarg = getopt_long (argc, argv, "atd", longopts, &index);

    if (iarg == -1)
      break;

    switch (iarg)
    {
      case 'a':
	Options::_alert = true;
	break;
      case 't':
	Options::_traffic = true;
	break;
      case 'd':
	Options::_debug = true;
	break;
      case '?':
	return ERR_ARG;
      default:
	return ERR_ERROR;
    }
  }
  return ERR_NONE;
}
