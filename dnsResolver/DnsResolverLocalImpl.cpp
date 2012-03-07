
#include <list>
#include <map>
#include <stdexcept>
#include <string>

#include <DnsResolver.h>
#include <DnsResolverLocalImpl.h>

using namespace std;

DnsResolverLocalImpl::DnsResolverLocalImpl()
{
}

// virtual
DnsResponse *DnsResolverLocalImpl::makeResponse(DnsRequest &request)
{
  DnsResponse *response;
  DnsMessage::MessageStatus retval = request.validate();
  if(retval != DnsMessage::MESSAGE_OK)
  {
    // Create an appropriate error response
    response = new DnsResponse(request);
    response->encodeError(retval);
    return response;
  }
  request.decode();

  IpAddrListType ipAddrList = getDnsResolution(request.getDomain());
  if(ipAddrList.empty())
  {
    Logger::getLogger()->error("No IP address found for domain: " + request.getDomain());
    response = new DnsResponse(request);
    response->encodeError(DnsMessage::MESSAGE_ERROR_DOMAIN_NOT_FOUND);
    return response;
  }

  response = new DnsResponse(request);

  response->addIpAddr(ipAddrList.begin(), ipAddrList.end());
  response->encode();

  return response;
}

// virtual
DnsResponse *DnsResolverLocalImpl::makeResponse(char *requestData, int requestLength)
{
  DnsRequest request(requestData, requestLength);

  return makeResponse(request);
}

// virtual
void DnsResolverLocalImpl::addDnsResolution(const string &domain, const string &ipAddress)
{
  map<string, IpAddrListType>::iterator iter = dnsMap_.find(domain);
  int ipAddressInt = DnsMessage::ipAddressToInt(ipAddress);
  if(iter == dnsMap_.end())
  {
    // Its a new entry
    DnsResolver::IpAddrListType ipList;
    ipList.push_back(ipAddressInt);

    dnsMap_.insert(make_pair(domain, ipList));
  }
  else
  {
    // the entry already exists, update its list
    iter->second.push_back(ipAddressInt);
  }
}

// virtual
DnsResolver::IpAddrListType DnsResolverLocalImpl::getDnsResolution(const string &domain)
{
  map<string, IpAddrListType >::const_iterator iter = dnsMap_.find(domain);
  if(iter == dnsMap_.end())
  {
    return IpAddrListType(); // empty list
  }
  else
  {
    return( iter->second );
  }
}
