
#include "ServerHandler.h"
#include "SocketServer.h"

ServerHandler::ServerHandler() :
  responseMessageLength_(0),
  isReadable_(true),
  clientAddr_(NULL)
{
  responseMessage_ = new char[SocketServer::MAX_MESSAGE_LENGTH];
}

ServerHandler::~ServerHandler()
{
  delete [] responseMessage_;
}
