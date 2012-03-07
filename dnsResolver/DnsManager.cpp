
#include <DnsManager.h>
#include <DnsResolver.h>
#include <DnsResponse.h>
#include <dnsUtils/Logger.h>

DnsManager::DnsManager(DnsResolver &resolver) :
  logger_(Logger::getLogger()),
  resolver_(resolver)
{
}

//virtual
void DnsManager::handleMessage(char *msg, int msgLength, struct sockaddr_in *clientAddr)
{
  DnsResponse *response = resolver_.makeResponse(msg, msgLength);

  responseMessageLength_ = response->getMessageLength();
  memcpy(responseMessage_, response->getMessage(), responseMessageLength_);
  delete response;

  clientAddr_ = clientAddr;  // Be carefull here, whats being pointed to shouldnt change
  isReadable_ = false;       // Set this handler to write mode
}
