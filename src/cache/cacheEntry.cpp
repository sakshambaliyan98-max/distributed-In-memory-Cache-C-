#include "cache/CacheEntry.h"

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

const string& CacheEntry::getValue() const
{
    return value_;
}

void CacheEntry::setValue(const string& value)
{
    value_ = value;
}

}