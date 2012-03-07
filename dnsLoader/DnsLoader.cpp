
#include <stdexcept>

#include <DnsLoader.h>
#include <DnsLoaderFormat.h>
#include <DnsLoaderMedium.h>

#include <dnsResolver/DnsResolver.h>

using namespace std;

DnsLoader::DnsLoader() :
  format_(NULL),
  medium_(NULL)
{
}

// virtual
DnsLoader::~DnsLoader()
{
  delete format_;
  delete medium_;
}

void DnsLoader::setDnsLoaderFormat(DnsLoaderFormat *format)
{
  format_ = format;
  format_->getNextDnsResolution();
}

void DnsLoader::setDnsLoaderMedium(DnsLoaderMedium *medium)
{
  medium_ = medium;
}

void DnsLoader::load(DnsResolver *resolver)
{
  if(format_ == NULL)
  {
    throw invalid_argument("DnsLoader DnsLoaderFormat is NULL");
  }

  if(medium_ == NULL)
  {
    throw invalid_argument("DnsLoader DnsLoaderMedium is NULL");
  }

  medium_->load();
  while(medium_->hasMore())
  {
    // parse returns false for entries that shouldnt be parsed: comments, etc
    if(format_->parse(medium_->getEntry()))
    {
      // Each medium entry may have several pieces of information
      while(format_->hasMore())
      {
        DnsLoaderFormat::DnsResolutionType dnsRes = format_->getNextDnsResolution();
        resolver->addDnsResolution(
            dnsRes.first,
            dnsRes.second);
      }
    }
  }
}
