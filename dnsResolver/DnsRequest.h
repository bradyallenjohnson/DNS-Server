
#ifndef DNS_REQUEST_H
#define DNS_REQUEST_H

#include <string>

#include <dnsResolver/DnsMessage.h>

using namespace std;

//
// A class representation of a DnsRequest
// Only server side usage has been implemented
//
class DnsRequest : public DnsMessage
{
  public:
    DnsRequest();
    DnsRequest(char *rawRequestData, int requestLength);

    string getDomain();
    virtual DnsMessage::MessageStatus validate();

    virtual void decode(); // str to object: parse raw string
    virtual void encode(); // object to str: make raw string

  private:
    string domain_;
};

#endif
