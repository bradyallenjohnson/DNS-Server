
#include <ServerHandler.h>
#include <SocketServer.h>

ServerHandler::ServerHandler() :
  clientAddr_(NULL),
  isReadable_(true),
  responseMessageLength_(0)
{
  responseMessage_ = new char[SocketServer::MAX_MESSAGE_LENGTH];
}

ServerHandler::~ServerHandler()
{
  delete [] responseMessage_;
}
