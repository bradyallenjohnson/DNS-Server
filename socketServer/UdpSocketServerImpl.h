
#ifndef UDP_SOCKET_SERVER_IMPL_H
#define UDP_SOCKET_SERVER_IMPL_H

#include "SocketServer.h"

//
// Concrete UDP implementation of SocketServer
//
class UdpSocketServerImpl : public SocketServer
{
  public:
    UdpSocketServerImpl();
    UdpSocketServerImpl(int listenPort, int timeout_millis = 0);

  protected:
    virtual bool initializeSpecific();
    virtual int readSpecific(char *buffer);
    virtual int writeSpecific(char *buffer, int bufLength);

};

#endif
