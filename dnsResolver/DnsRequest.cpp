
#include <stdexcept>
#include <string>

#include <DnsMessage.h>
#include <DnsRequest.h>

#include <dnsUtils/Logger.h>

using namespace std;

DnsRequest::DnsRequest()
{
}

DnsRequest::DnsRequest(char *rawMessageData, int requestLength) :
  DnsMessage(rawMessageData, requestLength)
{
}

string DnsRequest::getDomain()
{
  // lazy parsing, parse the raw message data as needed
  if(domain_.empty())
  {
    decode();
  }

  return domain_;
}

// Get the domain from the rawMessageData_
// virtual
void DnsRequest::decode()
{
  if(! domain_.empty())
  {
    // already decoded
    return;
  }

  if(validate() != MESSAGE_OK)
  {
    // TODO throw an exception
    logger_->error("DnsRequest::decode validate failed");
  }

  if(rawMessageDataLength_ < 1)
  {
    // TODO throw an exception
    logger_->error("DnsRequest::decode no rawMessageData to decode");
  }

  int length;
  getDomainName(rawMessageData_ + HEADER_LENGTH, domain_, &length);

  if(logger_->isDebug())
  {
    logger_->debug("DnsRequest::decode Domain received: " + domain_);
  }
}

// virtual
DnsMessage::MessageStatus DnsRequest::validate()
{
  if(validated_)
  {
    return MESSAGE_OK;
  }

  if(rawMessageDataLength_ < 1)
  {
    // TODO throw an exception
    logger_->error("DnsRequest::validate no rawMessageData to decode");
  }

  //
  // validate the header
  //

  // QR bit
  if((rawMessageData_[2] & QR_BIT_RESPONSE) != 0x00)
  {
    logger_->error("Invalid header: QR bit set to Response");
    return(MESSAGE_ERROR_HEADER_FORMAT);
  }

  // OPCODE
  if((rawMessageData_[2] & OPCODE_MASK) != 0x00)
  {
    logger_->error("Invalid header: Only QUERY OPCODE supported");
    return(MESSAGE_ERROR_NOT_SUPPORTED);
  }

  // AA bit
  if((rawMessageData_[2] & AA_BIT_ON) != 0x00)
  {
    logger_->error("Invalid header: AA bit is response only");
    return(MESSAGE_ERROR_HEADER_FORMAT);
  }

  // TC bit
  if((rawMessageData_[2] & TC_BIT_ON) != 0x00)
  {
    logger_->error("Invalid header: TC bit is response only");
    return(MESSAGE_ERROR_HEADER_FORMAT);
  }

  // RD bit, doesnt matter, we'll always reply with RD off

  // RA bit
  if((rawMessageData_[3] & RA_BIT_ON) != 0x00)
  {
    logger_->error("Invalid header: RA bit is response only");
    return(MESSAGE_ERROR_HEADER_FORMAT);
  }

  // Z bits
  if((rawMessageData_[3] & Z_BITS_MASK) != 0x00)
  {
    logger_->error("Invalid header: Z bits not supported");
    return(MESSAGE_ERROR_NOT_SUPPORTED);
  }

  // RCODE
  if((rawMessageData_[3] & RCODE_MASK) != 0x00)
  {
    logger_->error("Invalid header: RCODE is response only");
    return(MESSAGE_ERROR_HEADER_FORMAT);
  }

  // QDCOUNT
  if(rawMessageData_[4] != 0x00 && rawMessageData_[5] != 0x01)
  {
    logger_->error("Invalid header: only a QDCOUNT of 1 is supported");
    return(MESSAGE_ERROR_NOT_SUPPORTED);
  }

  //
  // Validate the question
  //
  int domainNameLength = getDomainNameLength(rawMessageData_ + HEADER_LENGTH);
  domainNameLength += HEADER_LENGTH;

  // QTYPE
  if(memcmp(rawMessageData_ + domainNameLength, &TYPE_HOST_ADDR, 2) != 0)
  {
    logger_->error("Invalid question: only QTYPE A is supported");
    return(MESSAGE_ERROR_NOT_SUPPORTED);
  }

  // QCLASS
  if(memcmp(rawMessageData_ + domainNameLength+2, &CLASS_IN, 2) != 0)
  {
    logger_->error("Invalid question: only QCLASS IN is supported");
    return(MESSAGE_ERROR_NOT_SUPPORTED);
  }

  validated_ = true;

  // All ok
  return MESSAGE_OK;
}

// this method is used for client side only, to be finished
// virtual
void DnsRequest::encode()
{
}
