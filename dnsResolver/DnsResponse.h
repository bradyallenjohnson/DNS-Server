
#ifndef DNS_RESPONSE_H
#define DNS_RESPONSE_H

#include <list>

#include <dnsResolver/DnsMessage.h>
#include <dnsResolver/DnsRequest.h>

using namespace std;

//
// A class representation of a DnsResponse
// Only server side functionality has been implemented
//
class DnsResponse : public DnsMessage
{
  public:
    DnsResponse();
    DnsResponse(DnsRequest &dnsRequest);
    ~DnsResponse();

    typedef list<unsigned int> IpAddrListType;
    typedef IpAddrListType::const_iterator IpAddrListIteratorType;

    DnsResponse::IpAddrListIteratorType getIpAddrListBegin();
    DnsResponse::IpAddrListIteratorType getIpAddrListEnd();

    void setRawRequestData(char *requestData, int requestDataLength);
    void addIpAddr(unsigned int ipAddr);
    void addIpAddr(IpAddrListIteratorType iter1, IpAddrListIteratorType iter2);
    void encodeError(DnsMessage::MessageStatus errorCode);

    virtual DnsMessage::MessageStatus validate();
    virtual char *getMessage();
    virtual void encode(); // object to str: make raw string
    virtual void decode(); // str to object: parse raw string

  private:
    IpAddrListType ipAddrList_;
    char *rawRequestData_;
    int rawRequestDataLength_;
};

#endif
