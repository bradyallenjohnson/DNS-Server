#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <sys/time.h>

#include "Logger.h"

using namespace std;

// Static initializations
Logger *Logger::instance_ = NULL;
Logger::LoggingLevel Logger::loggingLevel_ = Logger::INFO;

Logger::Logger() :
  output_(cout)
{
  levelStrings_.insert(make_pair(Logger::ERROR, "[ERROR] "));
  levelStrings_.insert(make_pair(Logger::INFO,  "[INFO ] "));
  levelStrings_.insert(make_pair(Logger::DEBUG, "[DEBUG] "));
}

void Logger::log(Logger::LoggingLevel level, const string &msg)
{
  if(loggingLevel_ >= level)
  {
    gettimeofday(&timevalStruct_, NULL);
    output_
      << levelStrings_[level]
      << "[" << timevalStruct_.tv_sec
      << "," << timevalStruct_.tv_usec << "] "
      << msg
      << endl;
  }
}

void Logger::log(Logger::LoggingLevel level, const stringstream &msg)
{
  if(loggingLevel_ >= level)
  {
    log(level, msg.str());
  }
}

//
// Singleton instantiation of the logger
//
// static
Logger *Logger::getLogger()
{
  if(instance_ != NULL)
  {
    return instance_;
  }

  instance_ = new Logger();

  return(instance_);
}

// static
void Logger::setLoggingLevel(Logger::LoggingLevel level)
{
  loggingLevel_ = level;
}

// static
Logger::LoggingLevel Logger::getLoggingLevel()
{
  return(loggingLevel_);
}

