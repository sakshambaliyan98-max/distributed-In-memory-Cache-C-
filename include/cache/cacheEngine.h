#pragma once

#include <bits/stdc++.h>

using namespace std;

#include "common/status.h"
#include "cache/cacheConfig.h"
#include "cache/cacheStats.h"
#include "storage/IStorage.h"
#include "eviction/EvictionPolicy.h"

namespace cache
{

class CacheEngine
{
private:

    CacheConfig config_;

    unique_ptr<IStorage> storage_;

    unique_ptr<EvictionPolicy> evictionPolicy_;

    CacheStats stats_;

public:

    CacheEngine();

    CacheEngine(
        const CacheConfig& config,
        unique_ptr<IStorage> storage,
        unique_ptr<EvictionPolicy> evictionPolicy
    );

    const CacheStats& getStats() const;

    Status set(
        const Key& key,
        const Value& value
    );

    Status set(
        const Key& key,
        const Value& value,
        chrono::seconds ttl
    );

    optional<Value> get(
        const Key& key
    );

    Status erase(
        const Key& key
    );

    bool exists(
        const Key& key
    ) const;

    size_t size() const;

    void clear();
};

}