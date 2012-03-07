
#ifndef DNS_LOADER_MEDIUM_FILE_IMPL_H
#define DNS_LOADER_MEDIUM_FILE_IMPL_H

#include <fstream>
#include <string>

#include <dnsLoader/DnsLoaderMedium.h>

using namespace std;

//
// Concrete implementation of a DnsLoaderMedium
//
class DnsLoaderMediumFileImpl : public DnsLoaderMedium
{
  public:
    DnsLoaderMediumFileImpl(const string &fileName);
    ~DnsLoaderMediumFileImpl();

    virtual void load();
    virtual string getEntry();
    virtual bool hasMore();

  private:
    DnsLoaderMediumFileImpl();
    string dnsFileName_;
    ifstream *dnsFile_;
};

#endif
