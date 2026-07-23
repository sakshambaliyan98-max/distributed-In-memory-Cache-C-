#pragma once

#include <list>
#include <unordered_map>

#include "eviction/EvictionPolicy.h"

namespace cache
{

class LRUEvictionPolicy : public EvictionPolicy
{
private:

    std::list<Key> lruList_;

    std::unordered_map<
        Key,
        std::list<Key>::iterator
    > positions_;

public:

    LRUEvictionPolicy() = default;

    ~LRUEvictionPolicy() override = default;

    void onInsert(const Key& key) override;

    void onAccess(const Key& key) override;

    void onErase(const Key& key) override;

    std::optional<Key> evict() override;

    void clear() override;
};

}