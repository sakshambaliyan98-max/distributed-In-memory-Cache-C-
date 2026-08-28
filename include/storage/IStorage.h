#pragma once

#include <bits/stdc++.h>

using namespace std;

#include "common/status.h"
#include "common/types.h"
#include "cache/cacheEntry.h"

namespace cache
{

class IStorage
{
public:
    virtual vector<pair<Key, CacheEntry>> entries() const = 0;

    virtual ~IStorage() = default;

    virtual Status set(
        const Key& key,
        const CacheEntry& entry) = 0;

    virtual const CacheEntry* find(
        const Key& key) const = 0;

    virtual Status erase(
        const Key& key) = 0;

    virtual bool exists(
        const Key& key) const = 0;

    virtual void clear() = 0;

    virtual size_t size() const = 0;
};

}