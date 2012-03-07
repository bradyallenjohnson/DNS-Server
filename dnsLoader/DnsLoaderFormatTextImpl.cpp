
#include <map>
#include <string>

#include <DnsLoaderFormat.h>
#include <DnsLoaderFormatTextImpl.h>

#include <dnsUtils/Logger.h>

using namespace std;

static const char CHAR_1 = '1';
static const char CHAR_9 = '9';
static const string WHITESPACE = " \t"; // space and tabs

DnsLoaderFormatTextImpl::DnsLoaderFormatTextImpl()
{
  domainEntryListIter_ = domainEntryList_.end();
}

// virtual
bool DnsLoaderFormatTextImpl::parse(const string &entry)
{
  //Logger::getLogger()->debug("DnsLoaderFormatTextImpl::parse input: " + entry);

  if(entry.empty())
  {
    return false;
  }

  // If the line doesnt start with [1..9] inclusive, skip it
  char firstChar = entry.at(0);
  if(firstChar < CHAR_1 || firstChar > CHAR_9)
  {
    return false;
  }

  // Reset the List for a new set of entries
  if(!domainEntryList_.empty())
  {
    // clearing an empty container caused memory problems ???
    domainEntryList_.clear();
  }

  // Could use a tokenizer here too

  //
  // Typical /etc/hosts file
  //
  // ##
  // # Host Database
  // #
  // # localhost is used to configure the loopback interface
  // # when the system is booting.  Do not change this entry.
  // ##
  // 127.0.0.1       localhost thismachine
  // 255.255.255.255 broadcasthost
  //

  //
  // Get the IP Address
  //

  string::size_type startIndex = entry.find_first_of(WHITESPACE);
  if(startIndex == string::npos)
  {
    // TODO Invalid entry, should we throw an exception?
    Logger::getLogger()->error("DnsLoaderFormatTextImpl::parse cant find space: " + entry);
    return false;
  }

  ipAddress_ = entry.substr(0, startIndex);

  //
  // Get the Host name(s)
  //
  startIndex = entry.find_first_not_of(WHITESPACE, startIndex);
  string::size_type endIndex = entry.find_first_of(WHITESPACE, startIndex);

  // Get all of the domain names in this particular entry
  while(startIndex != string::npos)
  {
    int nchars = (endIndex == string::npos) ? string::npos : (endIndex-startIndex);
    domainEntryList_.push_back(entry.substr(startIndex, nchars));

    if(Logger::getLogger()->isDebug())
    {
      Logger::getLogger()->debug(
          "Loading ipAddress: [" + ipAddress_ + "] domain: [" + entry.substr(startIndex, nchars) + "]");
    }

    startIndex = entry.find_first_not_of(WHITESPACE, endIndex);
    endIndex = entry.find_first_of(WHITESPACE, startIndex);
  }

  // now set the iterator back to the beginning
  domainEntryListIter_ = domainEntryList_.begin();

  return true;
}

// virtual
bool DnsLoaderFormatTextImpl::hasMore()
{
  return(domainEntryListIter_ != domainEntryList_.end());
}

// virtual
DnsLoaderFormat::DnsResolutionType DnsLoaderFormatTextImpl::getNextDnsResolution()
{
  return make_pair(*domainEntryListIter_++, ipAddress_);
}

