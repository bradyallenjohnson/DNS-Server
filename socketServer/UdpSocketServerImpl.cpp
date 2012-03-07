
#include <sys/socket.h>

#include <stdexcept>

#include <SocketServer.h>
#include <UdpSocketServerImpl.h>

UdpSocketServerImpl::UdpSocketServerImpl()
{
}

UdpSocketServerImpl::UdpSocketServerImpl(int port) :
  SocketServer(port)
{
}

// virtual
void UdpSocketServerImpl::initializeSpecific()
{
  sockFd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockFd_ < 0)
  {
    throw runtime_error("Error initializing UDP socket");
  }

  if(bind(sockFd_, (struct sockaddr *) &serverAddress_, sizeof(struct sockaddr_in)) < 0)
  {
    throw runtime_error("Error binding socket");
  }

  if(logger_->isInfo())
  {
    stringstream logMsg;
    logMsg << "UDP server successfully bound to port: " << port_;
    logger_->info(logMsg);
  }
}

