
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include <stdlib.h> // atoi

#include <dnsLoader/DnsLoader.h>
#include <dnsLoader/DnsLoaderFormatTextImpl.h>
#include <dnsLoader/DnsLoaderMediumFileImpl.h>

#include <dnsResolver/DnsManager.h>
#include <dnsResolver/DnsResolver.h>
#include <dnsResolver/DnsResolverLocalImpl.h>

#include <dnsUtils/ConfigFileParser.h>
#include <dnsUtils/ConfigOptions.h>
#include <dnsUtils/Logger.h>

#include <socketServer/SocketServer.h>
#include <socketServer/UdpSocketServerImpl.h>

using namespace std;

//
// Some commonly used constants
//
const string ARG_CONFIG_FILE = "-c";
const string ARG_PORT = "-p";
const string ARG_DNS_FILE = "-d";
const string ARG_VERBOSE = "-v";
const string ARG_HELP = "-h";
const string OPTION_CONFIG_FILE            =  "configFile";
const string OPTION_DNS_MAPPING_FILE       =  "dnsMapFile";
const string OPTION_DNS_PORT               =  "dnsPort";
const string OPTION_LOADER_FORMAT_LIBNAME  =  "loaderFormat";
const string OPTION_LOADER_MEDIUM_LIBNAME  =  "loaderMedium";

// Help
void printUsage()
{
  cout << "\nUsage:\n"
       << "\t " << ARG_PORT << " <UDP port> Optional, default 1053\n"
       << "\t " << ARG_VERBOSE << " Verbose logging. Optional, default INFO\n"
       << "\t " << ARG_DNS_FILE << " <DNS mapping file> Optional, default /etc/hosts\n"
       << endl;
}

//
// Simple Command line parser
//
void parseCommandLine(int argc, char **argv, ConfigOptions &options)
{
  if(argc > 1)
  {
    for(int i = 1; i < argc; i++)
    {
      string argvStr = argv[i];
      if(ARG_CONFIG_FILE == argvStr)
      {
        options.setValue(OPTION_CONFIG_FILE, argv[++i]);
      }
      else if(ARG_PORT == argvStr)
      {
        options.setValue(OPTION_DNS_PORT, atoi(argv[++i]));
      }
      else if(ARG_DNS_FILE == argvStr)
      {
        options.setValue(OPTION_DNS_MAPPING_FILE, argv[++i]);
      }
      else if(ARG_VERBOSE == argvStr)
      {
        Logger::setLoggingLevel(Logger::DEBUG);
      }
      else if(ARG_HELP == argvStr)
      {
        printUsage();
        exit(1);
      }
      else
      {
        throw invalid_argument("Invalid command line argument: " + argvStr );
      }
    }
  }
}

//
// Main program
//
int main(int argc, char **argv)
{
  Logger *logger = Logger::getLogger();
  ConfigOptions options;

  // Set the default values
  options.setValue(OPTION_CONFIG_FILE,      "conf/dnsConfigFile.xml");
  options.setValue(OPTION_DNS_MAPPING_FILE, "/etc/hosts");
  options.setValue(OPTION_DNS_PORT,         1053);

  //
  // Parse the command line arguments
  //
  try
  {
    parseCommandLine(argc, argv, options);
  }
  catch(const exception &e)
  {
    logger->error(string("Error parsing command line: ") + e.what());
    printUsage();
    return 0;
  }

  //
  // Parse the config file
  //
  ConfigFileParser parser;
  try
  {
    parser.setConfigFile(options.getStringValue(OPTION_CONFIG_FILE));
    parser.parse(options);
  }
  catch( const exception &e )
  {
    logger->error("Error parsing config file: " + options.getStringValue(OPTION_CONFIG_FILE));
    return 0;
  }

  //
  // Create the DNS Loader
  //
  DnsLoader *loader = NULL;
  try
  {
    // For now dynamic loading of extensible libraries is not implemented. Only the
    // DnsLoaderFormatTextImpl and DnsLoaderMediumFileImpl classes are implemented
    // options.getValue(OPTION_LOADER_FORMAT_LIBNAME);
    // options.getvalue(OPTION_LOADER_MEDIUM_LIBNAME);

    loader = new DnsLoader();
    loader->setDnsLoaderFormat(new DnsLoaderFormatTextImpl());
    loader->setDnsLoaderMedium(new DnsLoaderMediumFileImpl(options.getStringValue(OPTION_DNS_MAPPING_FILE)));
  }
  catch( const exception &e )
  {
    logger->error(string("Error creating the DNS Loader: ") + e.what());
    return 0;
  }

  //
  // Create and initialze the DnsManager and DnsResolver
  //
  DnsManager *manager = NULL;
  DnsResolver *resolver = NULL;
  try
  {
    resolver = new DnsResolverLocalImpl();
    loader->load(resolver);
    manager = new DnsManager(*resolver);
  }
  catch( const exception &e )
  {
    logger->error(string("Error Initializing the DnsResolver and DnsManager: ") + e.what());
    return 0;
  }

  //
  // Create and initialize the SocketServer
  //
  SocketServer *server = NULL;
  try
  {
    server = new UdpSocketServerImpl(options.getIntValue(OPTION_DNS_PORT));
    server->setHandler(manager);
    server->initialize();
  }
  catch( const exception &e )
  {
    logger->error(string("Error Initializing the SocketServer: ") + e.what());
    return 0;
  }

  //
  // Run the DNS Server
  //
  try
  {
    // This call blocks
    server->run();
  }
  catch( const exception &e )
  {
    logger->error(string("Error Running the DNS Server: ") + e.what());
    return 0;
  }

}

