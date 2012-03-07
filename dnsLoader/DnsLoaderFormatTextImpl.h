
#ifndef DNS_LOADER_FORMAT_TEXT_IMPL_H
#define DNS_LOADER_FORMAT_TEXT_IMPL_H

#include <list>
#include <string>

#include <dnsLoader/DnsLoaderFormat.h>

using namespace std;

//
// Concrete implementation of a DnsLoaderFormat
//   This class is to be used for a /etc/hosts text format.
//   Maybe it would be more correct to have an EtcHosts subclass. Future Release?
//
class DnsLoaderFormatTextImpl : public DnsLoaderFormat
{
  public:
    DnsLoaderFormatTextImpl();

    virtual bool parse(const string &entry);
    virtual bool hasMore();
    virtual DnsLoaderFormat::DnsResolutionType getNextDnsResolution();

  private:
    string ipAddress_;

    // The domains associated with the IpAddress
    list<string> domainEntryList_;
    // Stores where in the list we are for subsequent calls to getNextDnsResolution()
    list<string>::iterator domainEntryListIter_;
};

#endif
