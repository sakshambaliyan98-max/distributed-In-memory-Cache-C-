#include "eviction/LRUEvictionPolicy.h"

namespace cache
{

void LRUEvictionPolicy::onInsert(const Key& key)
{
    auto it = positions_.find(key);

    if(it != positions_.end())
    {
        onAccess(key);
        return;
    }

    lruList_.push_front(key);

    positions_.emplace(key, lruList_.begin());
}

void LRUEvictionPolicy::onAccess(const Key& key)
{
    auto it = positions_.find(key);

    if(it == positions_.end())
        return;

    lruList_.splice(
        lruList_.begin(),
        lruList_,
        it->second
    );

    it->second = lruList_.begin();
}

void LRUEvictionPolicy::onErase(const Key& key)
{
    auto it = positions_.find(key);

    if(it == positions_.end())
        return;

    lruList_.erase(it->second);

    positions_.erase(it);
}

std::optional<Key> LRUEvictionPolicy::evict()
{
    if(lruList_.empty())
        return std::nullopt;

    Key victim = lruList_.back();

    lruList_.pop_back();

    positions_.erase(victim);

    return victim;
}

void LRUEvictionPolicy::clear()
{
    lruList_.clear();
    positions_.clear();
}

}