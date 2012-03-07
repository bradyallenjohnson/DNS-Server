
#ifndef CONFIG_OPTIONS_H
#define CONFIG_OPTIONS_H

#include <map>
#include <string>

using namespace std;

//
// A simple Configuration Options holder class
//
class ConfigOptions
{
  public:
    void setValue(const string &, const string &);
    void setValue(const string &, int);
    // Cant overload on return type
    string getStringValue(const string &);
    int getIntValue(const string &);

  private:
    map<string, string> strMap_;
    map<string, int> intMap_;
};

#endif
