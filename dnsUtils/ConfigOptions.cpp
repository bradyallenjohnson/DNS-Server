
#include <stdexcept>
#include <map>
#include <string>

#include <ConfigOptions.h>

void ConfigOptions::setValue(const string &key, const string &value)
{
    strMap_[key] = value;
}

void ConfigOptions::setValue(const string &key, int value)
{
    intMap_[key] = value;
}

string ConfigOptions::getStringValue(const string &key)
{
  map<string, string>::iterator iter = strMap_.find(key);
  if(iter == strMap_.end())
  {
    throw invalid_argument(string("String Config option not found: ") + key);
  }
  else
  {
    return( iter->second );
  }
}

int ConfigOptions::getIntValue(const string &key)
{
  map<string, int>::iterator iter = intMap_.find(key);
  if(iter == intMap_.end())
  {
    throw invalid_argument(string("Int Config option not found: ") + key);
  }
  else
  {
    return( iter->second );
  }
}

