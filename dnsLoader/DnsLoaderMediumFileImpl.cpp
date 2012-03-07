
#include <fstream>
#include <stdexcept>
#include <string>

#include <DnsLoaderMedium.h>
#include <DnsLoaderMediumFileImpl.h>
#include <dnsUtils/Logger.h>

using namespace std;

DnsLoaderMediumFileImpl::DnsLoaderMediumFileImpl(const string &dnsFileName) :
  dnsFile_(NULL),
  dnsFileName_(dnsFileName)
{
}

DnsLoaderMediumFileImpl::~DnsLoaderMediumFileImpl()
{
  if(dnsFile_ != NULL)
  {
    dnsFile_->close();
  }
}

// virtual
void DnsLoaderMediumFileImpl::load()
{
  dnsFile_ = new ifstream(dnsFileName_.c_str());
  if(!dnsFile_)
  {
    throw invalid_argument("DnsLoaderMediumFileImpl::load file cannot be opened: " + dnsFileName_);
  }

  Logger::getLogger()->info("DNS resolution information being loaded from file: " + dnsFileName_);
}

// virtual
string DnsLoaderMediumFileImpl::getEntry()
{
  string entry;
  getline(*dnsFile_, entry);

  return entry;
}

// virtual
bool DnsLoaderMediumFileImpl::hasMore()
{
  return(!dnsFile_->eof());
}
