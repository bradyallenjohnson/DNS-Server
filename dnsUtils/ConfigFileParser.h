
#ifndef CONFIG_FILE_PARSER_H
#define CONFIG_FILE_PARSER_H

#include <string>

#include <dnsUtils/ConfigOptions.h>

using namespace std;

//
// An XML Configuration File Parser
// Not implemented yet.
//
class ConfigFileParser
{
  public:
    void setConfigFile(const string & configFile);
    void parse(ConfigOptions &options);
};

#endif
