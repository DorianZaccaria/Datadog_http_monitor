#include <pcap.h>

#include "sniffer.hh"
#include "options.hh"
#include "logger.hh"

Sniffer*			Sniffer::_singleton = NULL;

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

int
Sniffer::init ()
{
  char				errbuf[PCAP_ERRBUF_SIZE];

  _dev = pcap_lookupdev (errbuf);
  if (_dev == NULL)
  {
    Logger::log (Logger::ERROR, "Couldn't find device: ", errbuf);
    return(ERR_SNF);
  }

  return(ERR_NONE);
}
