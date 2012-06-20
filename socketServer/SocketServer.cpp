
#include <iostream>
#include <stdio.h>
#include <string.h>

#include <sys/select.h> 
#include <sys/types.h> 
#include <netinet/in.h>
#include <time.h>

#include "SocketServer.h"
#include "ServerHandler.h"

const int SocketServer::MAX_MESSAGE_LENGTH = 2048;

SocketServer::SocketServer() :
  handler_(NULL),
  port_(-1),
  sockFd_(-1),
  isDebug_(false),
  timeout_msec_(0),
  tval_(NULL),
  runServer_(true)
{
}

SocketServer::SocketServer(int port, int timeout_millis /*defaults 0*/) :
  handler_(NULL),
  port_(port),
  sockFd_(-1),
  isDebug_(false),
  timeout_msec_(timeout_millis),
  tval_(NULL),
  runServer_(true)
{
}

SocketServer::~SocketServer()
{
  delete handler_;
  if(sockFd_ >= 0)
  {
    close(sockFd_);
  }
  if(tval_ != NULL)
  {
    delete tval_;
  }
}

bool SocketServer::initialize()
{
  // We're always going to use IP
  memset(&serverAddress_, 0, sizeof(struct sockaddr_in));
  serverAddress_.sin_family = AF_INET;
  serverAddress_.sin_addr.s_addr = INADDR_ANY;
  serverAddress_.sin_port = htons(port_);

  clientAddrLen_ = sizeof(struct sockaddr_in);
  memset((void *) &clientAddress_, 0, sizeof(struct sockaddr_in));

  if(timeout_msec_ > 0)
  {
    tval_ = new struct timeval();
    if(timeout_msec_ > 999)
    {
      tval_->tv_sec  = timeout_msec_/1000;
      tval_->tv_usec = (timeout_msec_ - (tval_->tv_sec*1000))*1000;
    }
    else
    {
      tval_->tv_sec = 0;
      tval_->tv_usec = timeout_msec_*1000;
    }
    timeout_sec_  = tval_->tv_sec;
    timeout_usec_ = tval_->tv_usec;
  }

  // Template method design pattern
  // Perform common operations, then let concrete subclasses do their part
  return initializeSpecific();
}

// NOTICE: This method call blocks
// virtual
void SocketServer::run(int numMessagesToRead /*default 0*/)
{
  int numMessagesRead = 0;
  fd_set socketFdReadSet, socketFdWriteSet, socketFdErrorSet;
  int nready;
  char msgBuffer[MAX_MESSAGE_LENGTH];

  // perform anything the SocketServer Implementations may
  // need to do before starting the loop, like a TCP accept
  if(!runSpecific())
  {
    return;
  }

  FD_ZERO(&socketFdReadSet);
  FD_ZERO(&socketFdWriteSet);
  FD_ZERO(&socketFdErrorSet);

  while((numMessagesToRead == 0 || numMessagesRead < numMessagesToRead) && runServer_)
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
    if((nready = select(sockFd_+1, &socketFdReadSet, &socketFdWriteSet, &socketFdErrorSet, tval_)) < 0)
    {
      std::cerr << "Error in select";
    }

    if(nready == 0 && tval_ != NULL)
    {
      handler_->handleTimeout();
      // tval_ is reset to the number of seconds not slept
      tval_->tv_sec  = timeout_sec_;
      tval_->tv_usec = timeout_usec_;
      numMessagesRead++;
    }

    // Socket is ready to read
    if(FD_ISSET(sockFd_, &socketFdReadSet))
    {
      int numBytesRead = readSpecific(msgBuffer);
      numMessagesRead++;

      if(isDebug())
      {
        std::cout << "Received a message of size: " << numBytesRead;
      }

      handler_->handleMessage(msgBuffer, numBytesRead, &clientAddress_);
    }

    // Socket is ready to write
    if(FD_ISSET(sockFd_, &socketFdWriteSet))
    {
      int numBytesWritten = writeSpecific(handler_->getMessage(), handler_->getMessageLength());

      if(isDebug())
      {
        std::cout << "Replied with a message of size: " << numBytesWritten;
      }
    }

    // Socket has an error
    if(FD_ISSET(sockFd_, &socketFdErrorSet))
    {
      // TODO should we pass this to the handler too?

      std::cerr << "Error on socket";
      close(sockFd_);
      sockFd_ = -1;
    }
  }
}

