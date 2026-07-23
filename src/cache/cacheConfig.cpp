#include "cache/cacheConfig.h"

using namespace std;

namespace cache
{

CacheConfig::CacheConfig(size_t capacity)
    : capacity_(capacity)
{
}

size_t CacheConfig::getCapacity() const
{
    return capacity_;
}

}