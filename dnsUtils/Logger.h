
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include <sys/time.h>

using namespace std;

//
// A simple Singleton Logger
//
class Logger
{
  public:
    static Logger *getLogger();

    enum LoggingLevel
    {
      ERROR = 0,
      INFO,
      DEBUG
    };

    void log(Logger::LoggingLevel level, const stringstream &msg);
    void log(Logger::LoggingLevel level, const string &msg);

    inline void error(const stringstream &msg)  { log(Logger::ERROR, msg); }
    inline void error(const string &msg)        { log(Logger::ERROR, msg); }

    inline void info(const stringstream &msg)   { log(Logger::INFO, msg); }
    inline void info(const string &msg)         { log(Logger::INFO, msg); }

    inline void debug(const stringstream &msg)  { log(Logger::DEBUG, msg); }
    inline void debug(const string &msg)        { log(Logger::DEBUG, msg); }

    static void setLoggingLevel(Logger::LoggingLevel level);
    static Logger::LoggingLevel getLoggingLevel();

    inline bool isError() const { return(true); }
    inline bool isInfo()  const { return(loggingLevel_ != Logger::ERROR); }
    inline bool isDebug() const { return(loggingLevel_ == Logger::DEBUG); }

  private:
    Logger();
    ostream &output_;
    map<int, const string> levelStrings_;
    struct timeval timevalStruct_;
    static LoggingLevel loggingLevel_;
    static Logger *instance_;
};

#endif
