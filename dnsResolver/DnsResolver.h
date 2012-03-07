
#ifndef DNS_RESOLVER_H
#define DNS_RESOLVER_H

#include <list>
#include <string>

#include <dnsResolver/DnsRequest.h>
#include <dnsResolver/DnsResponse.h>

using namespace std;

//
// Abstract base class for resolving DNS addresses
// Only server side functionality has been included
// Concrete implementations should resolve address accordingly:
//     locally, contacting other DNS servers, DB, etc
//
class DnsResolver
{
  public:
    DnsResolver();
    ~DnsResolver();

    typedef list<unsigned int> IpAddrListType;
    typedef IpAddrListType::const_iterator IpAddrListIteratorType;

    // Given a DnsRequest object, get a Raw DNS response string
    virtual DnsResponse *makeResponse(DnsRequest &requestData) = 0;

    // Given a Raw DNS request string get a Raw DNS response string
    virtual DnsResponse *makeResponse(char *requestData, int requestLength) = 0;

    // Populate the dnsResolutionMap
    virtual void addDnsResolution(const string &domain, const string &ipAddress) = 0;

    // Directly query the dnsResolutionMap
    virtual IpAddrListType getDnsResolution(const string &domain) = 0;
};

#endif
