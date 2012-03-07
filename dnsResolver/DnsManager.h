
#ifndef DNS_MANAGER_H
#define DNS_MANAGER_H

#include <dnsResolver/DnsResolver.h>
#include <dnsUtils/Logger.h>
#include <socketServer/ServerHandler.h>
#include <socketServer/SocketServer.h>

//
// DnsManager class that orchestrates dns resolution
// Implements the SocketServer handler interface to handle incoming requests
//
// Simply calls a DnsResolver to get a response. This way the DnsResolver only deals with 
// DNS specifics and doesnt need to know about handling SocketServer messages, etc.
//
class DnsManager : public ServerHandler
{
  public:
    DnsManager(DnsResolver &resolver);

    // Implementation of SocketServer methods
    virtual void handleMessage(char *msg, int msgLength, struct sockaddr_in *clientAddr);

  private:
    DnsManager();
    DnsResolver &resolver_;
    Logger *logger_;
};

#endif
