
#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <sys/types.h> 
#include <netinet/in.h>

#include <dnsUtils/Logger.h>
#include <socketServer/ServerHandler.h>

//
// Abstract base class SocketServer
// Concrete implementations are UdpSocketServer and TcpSocketServer
//
class SocketServer
{
  public:
    SocketServer();
    SocketServer(int listenPort);
    virtual ~SocketServer();

    inline int getPort() {return port_;}
    inline void setPort(int port) {port_ = port;}
    inline void setHandler(ServerHandler *handler) {handler_ = handler;}

    void initialize();

    // NOTICE: This method call BLOCKS
    virtual void run();

    const static int MAX_MESSAGE_LENGTH;

  protected:
    // Template method design pattern
    virtual void initializeSpecific() = 0;

    ServerHandler *handler_;
    int port_;
    int sockFd_;
    struct sockaddr_in serverAddress_;
    Logger *logger_;
};

#endif
