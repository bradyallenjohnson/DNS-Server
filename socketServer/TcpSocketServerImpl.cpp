
#include <iostream>

#include <sys/socket.h>

#include "SocketServer.h"
#include "TcpSocketServerImpl.h"

TcpSocketServerImpl::TcpSocketServerImpl()
{
}

TcpSocketServerImpl::TcpSocketServerImpl(int port, int timeout_millis /*default 0*/) :
  SocketServer(port, timeout_millis)
{
}

// virtual
bool TcpSocketServerImpl::initializeSpecific()
{
  sockFd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sockFd_ < 0)
  {
    std::cerr << "Error initializing TCP socket" << std::endl;
    return false;
  }

  if(bind(sockFd_, (struct sockaddr *) &serverAddress_, sizeof(struct sockaddr_in)) < 0)
  {
    std::cerr << "Error binding socket" << std::endl;
    return false;
  }

  if(listen(sockFd_, 5) < 0)
  {
    std::cerr << "Error listening on socket" << std::endl;
    return false;
  }

  if(isDebug())
  {
    std::cout << "TCP server successfully bound to port: " << port_;
  }

  return true;
}

// virtual
// Blocking call
bool TcpSocketServerImpl::runSpecific()
{
  // TODO Currently this TCP server only accepts one client connection.
  //      Ideally, it should allow connections from any client,
  //      preferably in a multi-threaded manner.

  // Should be multi threaded since its a blocking call and this method shouldnt block
  int newsockfd = accept(sockFd_, (struct sockaddr *) &clientAddress_, &clientAddrLen_);
  if (newsockfd < 0)
  {
    std::cerr << "ERROR on accept" << std::endl;
    return false;
  }

  return true;
}

// virtual
int TcpSocketServerImpl::readSpecific(char *buffer)
{
  // TODO need to check if more bytes are avail to be read
  int numBytesRead =
        read(
            sockFd_,
            buffer,
            MAX_MESSAGE_LENGTH);

  return numBytesRead;
}

// virtual
int TcpSocketServerImpl::writeSpecific(char *buffer, int bufLength)
{
  int numBytesWritten = 
        write(
            sockFd_,
            buffer,
            bufLength);

  return numBytesWritten;
}

