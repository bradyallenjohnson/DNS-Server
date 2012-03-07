
#include <list>

#include <DnsMessage.h>
#include <DnsRequest.h>
#include <DnsResponse.h>

using namespace std;

DnsResponse::DnsResponse()
{
}

DnsResponse::DnsResponse(DnsRequest &request) :
  rawRequestData_(request.getMessage()),
  rawRequestDataLength_(request.getMessageLength())
{
}

DnsResponse::~DnsResponse()
{
  delete [] rawMessageData_;
}

char *DnsResponse::getMessage()
{
  // In case encode hasnt been explicitly called,
  // encode which goes from object to data string
  encode();
  return rawMessageData_;
}

void DnsResponse::setRawRequestData(char *requestData, int requestDataLength)
{
  rawRequestData_ = requestData;
  rawRequestDataLength_ = requestDataLength;
}

// Used by Client side
DnsResponse::IpAddrListIteratorType DnsResponse::getIpAddrListBegin()
{
  // Lazy parsing, parse when needed
  if(ipAddrList_.empty())
  {
    decode();
  }

  return ipAddrList_.begin();
}

// Used by Client side
DnsResponse::IpAddrListIteratorType DnsResponse::getIpAddrListEnd()
{
  // Lazy parsing, parse when needed
  if(ipAddrList_.empty())
  {
    decode();
  }

  return ipAddrList_.end();
}

void DnsResponse::addIpAddr(unsigned int ipAddr)
{
  ipAddrList_.push_back(ipAddr);
}

// Add the list to this list
void DnsResponse::addIpAddr(IpAddrListIteratorType iter1, IpAddrListIteratorType iter2)
{
  ipAddrList_.insert(ipAddrList_.end(), iter1, iter2);
}

// Used Client side
// When a response has been received, decode it
// virtual
void DnsResponse::decode()
{
  // validate();
  // parse rawMessageData and populate the ipAddrList
}

// Used Server side
// When a request has been received and its data has been
// stored in this object, prepare/encode the response
// virtual
void DnsResponse::encode()
{
  if(rawMessageDataLength_ > 0)
  {
    // already been encoded
    return;
  }

  if(rawRequestDataLength_ < 1)
  {
    // TODO exception
    logger_->error("Cant create a response since no request data has been set.");
  }

  if(ipAddrList_.empty())
  {
    // TODO exception
    logger_->error("Cant create a response since no ip addresses have been loaded.");
  }

  // get the domain length
  char domainLength = (char) getDomainNameLength(rawRequestData_ + HEADER_LENGTH);

  short ancount = ipAddrList_.size();
  // AnswerLength = domainLength + TYPE(2) + CLASS(2) + TTL(4) + RDATALen(2) + RDATA(4)
  rawMessageDataLength_ = rawRequestDataLength_ + (ancount*(domainLength+14));
  rawMessageData_ = new char[rawMessageDataLength_];
  bzero(rawMessageData_, rawMessageDataLength_);

  // Copy the request message header and question
  memcpy(rawMessageData_, rawRequestData_, rawRequestDataLength_);

  //
  // Set the header response values
  //

  // QR Response byte 2
  rawMessageData_[2] |= QR_BIT_RESPONSE;

  // RA Response byte 3, its ok if we turn off the rest of the bits of this byte
  rawMessageData_[3] = RA_BIT_OFF;

  // RCODE Response byte 3
  rawMessageData_[3] |= RCODE_NO_ERROR;

  // Set the ANCOUNT after we've processed the ipAddresses
  switch2bytes(rawMessageData_+6, ancount);

  // Set the NSCOUNT and ARCOUNT both to 0
  memset(rawMessageData_+8, 0x00, 4);

  //
  // Now add the Answer(s)
  //
  int index = rawRequestDataLength_;
  IpAddrListIteratorType iter = ipAddrList_.begin();
  for( ; iter != ipAddrList_.end(); ++iter)
  {
    // Name(domain), copy it from the request
    memcpy(rawMessageData_+index, rawMessageData_+HEADER_LENGTH, domainLength);
    index += domainLength;

    // TYPE
    memcpy(rawMessageData_+index, &TYPE_HOST_ADDR, 2);
    index += 2;

    // CLASS
    memcpy(rawMessageData_+index, &CLASS_IN, 2);
    index += 2;

    // TTL
    memcpy(rawMessageData_+index, &TTL, 4);
    index += 4;

    // RDLENGTH
    switch2bytes(rawMessageData_+index, IP_ADDR_LENGTH);
    index += 2;

    // RDATA
    memcpy(rawMessageData_+index, &(*iter), 4);
    index += 4;
  }
}

void DnsResponse::encodeError(DnsMessage::MessageStatus errorCode)
{
  if(rawRequestDataLength_ < 1)
  {
    // TODO exception
    logger_->error("Cant create a response since no request data has been set.");
  }

  //
  // Just copy the request message and set the RCODE accordingly
  //

  rawMessageDataLength_ = rawRequestDataLength_;
  rawMessageData_ = new char[rawMessageDataLength_];
  bzero(rawMessageData_, rawMessageDataLength_);

  // Copy the request message header and question
  memcpy(rawMessageData_, rawRequestData_, rawRequestDataLength_);

  //
  // Set the header response values
  //

  // QR Response byte 2
  rawMessageData_[2] |= QR_BIT_RESPONSE;

  // RA Response byte 3, its ok if we turn off the rest of the bits of this byte
  rawMessageData_[3] = RA_BIT_OFF;

  // RCODE Response byte 3
  switch(errorCode)
  {
    case MESSAGE_ERROR_HEADER_FORMAT:
      rawMessageData_[3] |= RCODE_FORMAT_ERROR;
      break;

    case MESSAGE_ERROR_NOT_SUPPORTED:
      rawMessageData_[3] |= RCODE_NOT_IMPLEMENTED;
      break;

    case MESSAGE_ERROR_DOMAIN_NOT_FOUND:
      rawMessageData_[3] |= RCODE_NAME_ERROR;
      break;

    case MESSAGE_ERROR_SERVER_ERROR:
    default:
      rawMessageData_[3] |= RCODE_SERVER_FAILURE;
      break;
  }
}

// Not implemented, only needed for client usage
// virtual
DnsMessage::MessageStatus DnsResponse::validate()
{
  if(validated_)
  {
    return MESSAGE_OK;
  }

  return MESSAGE_ERROR_SERVER_ERROR;
}
