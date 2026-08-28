#pragma once

#include <bits/stdc++.h>

using namespace std;

#include <mutex>

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

    // Guards storage_, evictionPolicy_, and stats_ as a single unit -
    // they must stay consistent with each other, so a single coarse-grained
    // lock (rather than per-member locks) avoids torn/interleaved updates
    // when multiple TcpServer worker threads call into the engine concurrently.
    mutable std::mutex mutex_;

public:

    CacheEngine();

    CacheEngine(
        const CacheConfig& config,
        unique_ptr<IStorage> storage,
        unique_ptr<EvictionPolicy> evictionPolicy
    );

    // Returned by value (not reference): the copy is made while mutex_ is
    // still held, so callers never read stats_ concurrently with a writer.
    CacheStats getStats() const;

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