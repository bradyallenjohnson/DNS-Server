
#ifndef DNS_RESOLVER_LOCAL_IMPL_H
#define DNS_RESOLVER_LOCAL_IMPL_H

#include <map>
#include <string>

#include <dnsResolver/DnsRequest.h>
#include <dnsResolver/DnsResolver.h>
#include <dnsResolver/DnsResponse.h>

using namespace std;

//
// Concrete implementation of the DnsResolver
// Performs dns resolution locally
//
class DnsResolverLocalImpl : public DnsResolver
{
  public:
    DnsResolverLocalImpl();

    // Given a DnsRequest object, get a Raw DNS response string
    virtual DnsResponse *makeResponse(DnsRequest &requestData);

    // Given a Raw DNS request string get a Raw DNS response string
    virtual DnsResponse *makeResponse(char *requestData, int requestLength);

    // Populate the dnsResolutionMap
    virtual void addDnsResolution(const string &domain, const string &ipAddress);

    // Directly query the dnsResolutionMap
    virtual IpAddrListType getDnsResolution(const string &domain);

  private:
    map<string, IpAddrListType > dnsMap_;
};

#endif
