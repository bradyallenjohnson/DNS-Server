
#ifndef DNS_LOADER_H
#define DNS_LOADER_H

#include <dnsLoader/DnsLoaderFormat.h>
#include <dnsLoader/DnsLoaderMedium.h>

#include <dnsResolver/DnsResolver.h>

//
// Loads the Domain Name mapping information
//
class DnsLoader
{
  public:
    DnsLoader();
    virtual ~DnsLoader();

    // Orchestrates the loading of DNS data and stores it in a DnsResolver
    void load(DnsResolver *resolver);

    void setDnsLoaderFormat(DnsLoaderFormat *format);
    void setDnsLoaderMedium(DnsLoaderMedium *medium);

  private:
    DnsLoaderFormat *format_;
    DnsLoaderMedium *medium_;
};

#endif
