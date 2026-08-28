#pragma once

#include <unordered_map>

#include "storage/IStorage.h"

namespace cache
{

class HashMapStorage : public IStorage
{
private:

    std::unordered_map<Key, CacheEntry> storage_;

public:

    HashMapStorage();
    vector<pair<Key, CacheEntry>> entries() const override;

    Status set(
        const Key& key,
        const CacheEntry& entry
    ) override;

    const CacheEntry* find(
        const Key& key
    ) const override;

    Status erase(
        const Key& key
    ) override;

    bool exists(
        const Key& key
    ) const override;

    void clear() override;

    size_t size() const override;
};

}