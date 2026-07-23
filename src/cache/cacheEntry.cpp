#include "cache/cacheEntry.h"

using namespace std;

namespace cache
{

CacheEntry::CacheEntry()
{
}

CacheEntry::CacheEntry(const string& value)
    : value_(value)
{
}

CacheEntry::CacheEntry(
    const string& value,
    chrono::seconds ttl
)
    : value_(value)
{
    setTTL(ttl);
}

const string& CacheEntry::getValue() const
{
    return value_;
}

void CacheEntry::setValue(const string& value)
{
    value_ = value;
}

void CacheEntry::setTTL(chrono::seconds ttl)
{
    expiryTime_ = chrono::steady_clock::now() + ttl;
}

void CacheEntry::clearTTL()
{
    expiryTime_.reset();
}

bool CacheEntry::hasExpired() const
{
    if(!expiryTime_.has_value())
        return false;

    return chrono::steady_clock::now() >= *expiryTime_;
}

}