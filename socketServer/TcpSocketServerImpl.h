
#ifndef TCP_SOCKET_SERVER_IMPL_H
#define TCP_SOCKET_SERVER_IMPL_H

#include "SocketServer.h"

//
// Concrete TCP implementation of SocketServer
//
class TcpSocketServerImpl : public SocketServer
{
  public:
    TcpSocketServerImpl();
    TcpSocketServerImpl(int listenPort, int timeout_millis = 0);

  protected:
    virtual bool initializeSpecific();
    virtual int readSpecific(char *buffer);
    virtual int writeSpecific(char *buffer, int bufLength);
    virtual bool runSpecific();

};

#endif
