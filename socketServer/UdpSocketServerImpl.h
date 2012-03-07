
#ifndef UDP_SOCKET_SERVER_IMPL_H
#define UDP_SOCKET_SERVER_IMPL_H

#include <socketServer/SocketServer.h>

//
// Concrete UDP implementation of SocketServer
//
class UdpSocketServerImpl : public SocketServer
{
  public:
    UdpSocketServerImpl();
    UdpSocketServerImpl(int listenPort);

  protected:
    virtual void initializeSpecific();

};

#endif
