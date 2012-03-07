
#ifndef DNS_LOADER_FORMAT_H
#define DNS_LOADER_FORMAT_H

#include <string>

using namespace std;

//
// Abstract base class for Dns Loader Formats
//
class DnsLoaderFormat
{
  public:
    DnsLoaderFormat();

    typedef pair<string, string> DnsResolutionType;

    virtual bool parse(const string &entry) = 0;
    virtual bool hasMore() = 0;
    virtual DnsLoaderFormat::DnsResolutionType getNextDnsResolution() = 0;
};

#endif
