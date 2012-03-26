
#include <sys/select.h> 
#include <sys/types.h> 
#include <netinet/in.h>

#include <string.h> // bzero

#include <stdexcept>

#include <SocketServer.h>
#include <ServerHandler.h>

#include <dnsUtils/Logger.h>

const int SocketServer::MAX_MESSAGE_LENGTH = 1024;

SocketServer::SocketServer() :
  handler_(NULL),
  logger_(Logger::getLogger()),
  port_(-1),
  sockFd_(-1)
{
}

SocketServer::SocketServer(int port) :
  handler_(NULL),
  logger_(Logger::getLogger()),
  port_(port),
  sockFd_(-1)
{
}

SocketServer::~SocketServer()
{
  delete handler_;
  if(sockFd_ >= 0)
  {
    close(sockFd_);
  }
}

void SocketServer::initialize()
{
  // We're always going to use IP
  bzero((char *) &serverAddress_, sizeof(struct sockaddr_in));
  serverAddress_.sin_family = AF_INET;
  serverAddress_.sin_addr.s_addr = INADDR_ANY;
  serverAddress_.sin_port = htons(port_);

  // Template method design pattern
  // Perform common operations, then let concrete subclasses do their part
  initializeSpecific();
}

// NOTICE: This method call blocks
// virtual
void SocketServer::run()
{
  fd_set socketFdReadSet, socketFdWriteSet, socketFdErrorSet;
  int nready;
  char msgBuffer[MAX_MESSAGE_LENGTH];
  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(struct sockaddr_in);

  FD_ZERO(&socketFdReadSet);
  FD_ZERO(&socketFdWriteSet);
  FD_ZERO(&socketFdErrorSet);

  while(true)
  {

    // We could be more extensable by having a list of handlers and iterate it

    // The handler/socket is only readable or writable, not both
    if(handler_->isReadable())
    {
      FD_SET(sockFd_, &socketFdReadSet);
      FD_ZERO(&socketFdWriteSet);
    }
    else
    {
      FD_SET(sockFd_, &socketFdWriteSet);
      FD_ZERO(&socketFdReadSet);
    }

    // Always check for errors
    FD_SET(sockFd_, &socketFdErrorSet);

    // This call blocks
    if((nready = select(sockFd_+1, &socketFdReadSet, &socketFdWriteSet, &socketFdErrorSet, NULL)) < 0)
    {
      logger_->error("Error in select");
    }

    // Socket is ready to read
    if(FD_ISSET(sockFd_, &socketFdReadSet))
    {
      // TODO need to check if more bytes are avail to be read
      int numBytesRead =
        recvfrom(
            sockFd_,
            msgBuffer,
            MAX_MESSAGE_LENGTH,
            0, // flags
            (struct sockaddr *) &clientAddr,
            &clientAddrLen);

      if(logger_->isDebug())
      {
        stringstream logMsg;
        logMsg << "Received a message of size: " << numBytesRead;
        logger_->debug(logMsg);
      }

      handler_->handleMessage(msgBuffer, numBytesRead, &clientAddr);
    }

    // Socket is ready to write
    if(FD_ISSET(sockFd_, &socketFdWriteSet))
    {
      int numBytesWritten = 
        sendto(
            sockFd_,
            handler_->getMessage(),
            handler_->getMessageLength(),
            0, // flags
            (struct sockaddr *) handler_->getClientAddr(),
            clientAddrLen);

      if(logger_->isDebug())
      {
        stringstream logMsg;
        logMsg << "Replied with a message of size: " << numBytesWritten;
        logger_->debug(logMsg);
      }
    }

    // Socket has an error
    if(FD_ISSET(sockFd_, &socketFdErrorSet))
    {
      logger_->error("Error on socket");
      close(sockFd_);
      sockFd_ = -1;
    }
  }
}

