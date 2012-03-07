
#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include <sys/types.h> 
#include <netinet/in.h>

//
// Abstract base class ServerHandler
// handles SocketServer incoming messages
// For now centered around UDP
//
class ServerHandler
{
  public:
    ServerHandler();
    virtual ~ServerHandler();

    // Called when a message is read from a client
    virtual void handleMessage(char *msg, int msgLength, struct sockaddr_in *clientAddr) = 0;

    // Called to accept connections, default behavior is provided
    // WONT be used for now
    //virtual void handleConnect(int socketFd, const struct sockaddr_in &clientAddr, socklen_t clientAddrLen);

    // Called to get a message to write to the client
    inline char *getMessage()      {isReadable_ = true; return responseMessage_;}
    inline int getMessageLength()  {return responseMessageLength_;}
    inline bool isReadable()       {return isReadable_;}
    inline struct sockaddr_in *getClientAddr() {return clientAddr_;}

  protected:
    char *responseMessage_;
    int responseMessageLength_;
    bool isReadable_;
    struct sockaddr_in *clientAddr_;
};

#endif
