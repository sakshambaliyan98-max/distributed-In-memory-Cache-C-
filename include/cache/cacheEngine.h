#pragma once

#include<bits/stdc++.h>

using namespace std;

#include "cache/CacheConfig.h"
#include "cache/CacheEntry.h"
#include "cache/CacheStats.h"
#include "common/Status.h"
#include "common/Types.h"

namespace cache
{

class CacheEngine
{
private:

    unordered_map<Key, CacheEntry> storage_;

    CacheConfig config_;

    CacheStats stats_;

public:

    explicit CacheEngine(const CacheConfig& config = CacheConfig());

    Status set(const Key& key,
               const value& value);

    optional<value> get(const Key& key);

    Status erase(const Key& key);

    bool exists(const Key& key) const;

    void clear();

    size_t size() const;

    const CacheStats& getStats() const;
};

}