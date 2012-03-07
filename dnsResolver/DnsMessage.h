
#ifndef DNS_MESSAGE_H
#define DNS_MESSAGE_H

#include <string>

#include <dnsUtils/Logger.h>

using namespace std;

//
// Details for DnsRequests and DnsResponses
//
// Structure of a DNS Lookup message http://www.freesoft.org/CIE/Topics
//  +---------------------+
//  |        Header       |
//  +---------------------+
//  |       Question      | the question for the name server
//  +---------------------+
//  |        Answer       | RRs answering the question
//  +---------------------+
//  |      Authority      | RRs pointing toward an authority
//  +---------------------+
//  |      Additional     | RRs holding additional information
//  +---------------------+
//
// Header Request 6 bytes, Response 12 bytes
//              MSB[0] <--> LSB[15]
//   Byte 0,1: ID
//   Byte 2,3: QR[0] Opcode[1..4] AA[5] TC[6] RD[7] RA[8] Z[9..11] RCODE[12..15]
//   Byte 4,5: QDCOUNT unsigned 16 bit int specifying the number of entries in the question section
//   Byte 6,7: ANCOUNT unsigned 16 bit int specifying the number of resource records in the answer section
//   Byte 8,9: NSCOUNT unsigned 16 bit int specifying the number of name server resource records in the authority records section
//   Byte 10,11: ARCOUNT unsigned 16 bit int specifying the number of resource records in the additional records section
//

// Header Id set in request, copied to response
const static unsigned short HEADER_ID = 0xABCD;
// Query/Response bit
const static char QR_BIT_QUERY    = 0x00;
const static char QR_BIT_RESPONSE = 0x80;
// opcode set in request, copied to response (3-15 resvd future use)
const static char OPCODE_QUERY     = 0x00; // 0
const static char OPCODE_INV_QUERY = 0x02; // 1
const static char OPCODE_STATUS    = 0x04; // 2
const static char OPCODE_MASK      = 0x1E;
// Authoritative Answer, responses only
const static char AA_BIT_ON   = 0x20;
const static char AA_BIT_OFF  = 0x00;
// Truncation, responses only
const static char TC_BIT_ON   = 0x40;
const static char TC_BIT_OFF  = 0x00;
// Recursion desired, set in request, copied to response
const static char RD_BIT_ON   = 0x80;
const static char RD_BIT_OFF  = 0x00;
// Recursion available, response only
const static char RA_BIT_ON   = 0x01;
const static char RA_BIT_OFF  = 0x00;
// Z, reserved for future use, must be off
const static char Z_BITS_OFF  = 0x00;
const static char Z_BITS_MASK = 0xE0;
// rcode response only (6-15 resvd future use)
const static char RCODE_NO_ERROR        = 0x00; // 0
const static char RCODE_FORMAT_ERROR    = 0x01; // 1
const static char RCODE_SERVER_FAILURE  = 0x02; // 2
const static char RCODE_NAME_ERROR      = 0x03; // 3
const static char RCODE_NOT_IMPLEMENTED = 0x04; // 4
const static char RCODE_REFUSED         = 0x05; // 5
const static char RCODE_MASK            = 0x0F;
// qdcount number of question entries, always 1
const static char QDCOUNT    = 0x0001;

// TYPE values, only supporting A Host Address
const static unsigned short TYPE_HOST_ADDR = 0x0100;

// CLASS values, only supporting internet class
const static unsigned short CLASS_IN = 0x0100;

// TTL Time To Live
const static unsigned int TTL = 172800u;

// TTL Time To Live
const static unsigned short IP_ADDR_LENGTH = 0x0004;

const static int HEADER_LENGTH = 12;

const static char CHAR_PERIOD = '.';

class DnsMessage
{
  public:
    DnsMessage();
    DnsMessage(char *rawMessageData, int msgLength);
    virtual ~DnsMessage();

    enum MessageStatus
    {
      MESSAGE_OK = 0,
      MESSAGE_ERROR_HEADER_FORMAT,
      MESSAGE_ERROR_NOT_SUPPORTED,
      MESSAGE_ERROR_SERVER_ERROR,
      MESSAGE_ERROR_DOMAIN_NOT_FOUND
    };

    virtual char *getMessage();
    inline int getMessageLength() { return rawMessageDataLength_; }
    void getDomainName(char *data, string &domainName, int *domainNameLength);
    int getDomainNameLength(char *data);

    virtual DnsMessage::MessageStatus validate() = 0;
    virtual void decode() = 0;
    virtual void encode() = 0;

    // A few catchall methods
    static string ipAddressToString(int ipAddr);
    static int ipAddressToInt(const string &ipAddr);
    static int switch2bytes(char *dest, short src);

  protected:
    char *rawMessageData_;
    int rawMessageDataLength_;
    bool validated_;
    Logger *logger_;
};

#endif
