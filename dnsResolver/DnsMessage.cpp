
#include <stdexcept>
#include <string>

#include <dnsUtils/Logger.h>

#include <DnsMessage.h>

using namespace std;

DnsMessage::DnsMessage() :
  validated_(false),
  logger_(Logger::getLogger()),
  rawMessageData_(NULL),
  rawMessageDataLength_(0)
{
}

DnsMessage::DnsMessage(char *rawMessageData, int messageLength) :
  validated_(false),
  logger_(Logger::getLogger()),
  rawMessageData_(rawMessageData),
  rawMessageDataLength_(messageLength)
{
}

DnsMessage::~DnsMessage()
{
}

// virtual
char *DnsMessage::getMessage()
{
  return rawMessageData_;
}

void DnsMessage::getDomainName(char *data, string &domainName, int *domainNameLength)
{
  int i;
  char *domainPtr = data;
  *domainNameLength = 0;
  for(i = 0; (*domainPtr != 0x00) && i < 50; i++) // checks for infinite loop
  {
    (*domainNameLength) += ((*domainPtr) + 1);
    domainName.append(domainPtr+1, *domainPtr);
    domainPtr += ((*domainPtr) + 1);
    if(*domainPtr != 0x00)
    {
      domainName.append(&CHAR_PERIOD, 1);
    }
  }
  (*domainNameLength)++; // for the trailing zero byte

  if(i >= 50)
  {
    // TODO possible infinite loop detected
  }
}

int DnsMessage::getDomainNameLength(char *data)
{
  int i;
  char *domainPtr = data;
  int domainNameLength = 0;
  for(i = 0; (*domainPtr != 0x00) && i < 50; i++) // checks for infinite loop
  {
    domainNameLength += ((*domainPtr) + 1);
    domainPtr += ((*domainPtr) + 1);
  }
  domainNameLength++; // for the trailing zero byte

  if(i >= 50)
  {
    // TODO possible infinite loop detected
  }

  return domainNameLength;
}

// static
string DnsMessage::ipAddressToString(int ipAddr)
{
  // Not implemented
}

// static
int DnsMessage::ipAddressToInt(const string &ipAddr)
{
  int ipAddrInt;
  char *ipAddrPtr = (char *) &ipAddrInt;

  string::size_type startIndex = 0;
  for( int i = 0; i < 3; i++)
  {
    string::size_type endIndex = ipAddr.find(CHAR_PERIOD, startIndex);
    if(endIndex == string::npos)
    {
      // TODO throw an exception
      Logger::getLogger()->error("DnsMessage::ipAddressToInt invalid ipAddr: " + ipAddr);
      return 0x00000000;
    }
    // TODO do more exhaustive checking of address
    *(ipAddrPtr+i) = atoi(ipAddr.substr(startIndex, endIndex).c_str());
    startIndex = endIndex+1;
  }
  *(ipAddrPtr+3) = atoi(ipAddr.substr(startIndex, string::npos).c_str());

  stringstream logMsg;
  logMsg << "Converting IP addr: " << ipAddr << " to: " << ipAddrInt;
  Logger::getLogger()->debug(logMsg);

  return ipAddrInt;
}

// static
int DnsMessage::switch2bytes(char *destPtr, short src)
{
  *destPtr++ = src >> 8;
  *destPtr   = src;
}
