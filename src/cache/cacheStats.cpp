#include "cache/CacheStats.h"

using namespace std;

namespace cache
{

CacheStats::CacheStats()
    : hits_(0),
      misses_(0)
{
}

void CacheStats::recordHit()
{
    ++hits_;
}

void CacheStats::recordMiss()
{
    ++misses_;
}

uint64_t CacheStats::getHits() const
{
    return hits_;
}

uint64_t CacheStats::getMisses() const
{
    return misses_;
}

}