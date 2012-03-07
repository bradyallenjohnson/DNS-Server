
#ifndef DNS_LOADER_MEDIUM_H
#define DNS_LOADER_MEDIUM_H

#include <string>

using namespace std;

//
// Abstract base class for DNS Loader Mediums
//
class DnsLoaderMedium
{
  public:
    DnsLoaderMedium();

    virtual void load() = 0;
    virtual string getEntry() = 0;
    virtual bool hasMore() = 0;
};

#endif
