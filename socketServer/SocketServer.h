
#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <sys/types.h> 
#include <netinet/in.h>
#include <time.h>

#include "ServerHandler.h"

//
// Abstract base class SocketServer
// Concrete implementations are UdpSocketServer and TcpSocketServer
// The server is a simple single threaded server
//
class SocketServer
{
  public:
    SocketServer();
    SocketServer(int listenPort, int timeout_millis = 0);
    virtual ~SocketServer();

    inline int getPort() {return port_;}
    inline void setPort(int port) {port_ = port;}
    inline void setHandler(ServerHandler *handler) {handler_ = handler;}

    inline bool isDebug() {return isDebug_;}
    inline void setDebug(bool dbg) {isDebug_ = dbg;}

    bool initialize();

    // NOTICE: This method call may be BLOCKING
    // If numMessagesToRead is set, this call will execute until that number of messages is read
    // If numMessagesToRead is 0, then the call will block and run continuously
    virtual void run(int numMessagesToRead=0);
    virtual inline void stop() { runServer_ = false; }

    const static int MAX_MESSAGE_LENGTH;

  protected:
    // Template method design pattern
    virtual bool initializeSpecific() = 0;
    virtual int readSpecific(char *buffer) = 0;
    virtual int writeSpecific(char *buffer, int bufLength) = 0;

    virtual bool runSpecific() { return true; }

    ServerHandler *handler_;
    int port_;
    int sockFd_;
    struct sockaddr_in serverAddress_;
    struct sockaddr_in clientAddress_;
    socklen_t clientAddrLen_;
    bool isDebug_;
    int timeout_sec_;
    int timeout_msec_;
    int timeout_usec_;
    struct timeval *tval_;
    bool runServer_;
};

#endif
