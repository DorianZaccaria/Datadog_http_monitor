#include <ctime>
#include <chrono>
#include <iostream>

#include "logger.hh"
#include "options.hh"

Logger*				Logger::_singleton = NULL;

static std::string const cat_header[Logger::DEBUG + 1] =
{
  [Logger::ERROR] = "ERROR",
  [Logger::INFO] = "INFO",
  [Logger::TRAFFIC] = "TRAFFIC",
  [Logger::ALERT] = "ALERT",
  [Logger::DEBUG] = "DEBUG"
};

Logger::Logger ()
{
}

Logger::~Logger ()
{
}

void
Logger::init ()
{
  if (!Logger::_singleton)
    Logger::_singleton = new Logger ();
}

void
Logger::log (Logger::Category const			cat,
	     std::string const&				msg)
{
  std::chrono::system_clock::time_point			now;
  std::time_t						tt;
  std::streambuf*					buf = NULL;

  if (cat == ERROR)
    buf = std::cerr.rdbuf ();
  else
    buf = std::cout.rdbuf ();
  std::ostream						output (buf);

  if ((cat == DEBUG && !Options::_debug) ||
      (cat == TRAFFIC && !Options::_traffic) ||
      (cat == ALERT && !Options::_alert))
    return;

  now = std::chrono::system_clock::now ();
  tt = std::chrono::system_clock::to_time_t (now);

  output << "[" << tt << "]"
	 << "[" << cat_header[cat] << "] "
	 << msg << std::endl;
}

void
Logger::log (Logger::Category const			cat,
	     std::string const&				msg,
	     char const*				extra)
{
  std::string						tmp (extra);

  log (cat, msg + tmp);
}
