#include "storage/HashMapStorage.h"

namespace cache
{

HashMapStorage::HashMapStorage() = default;

Status HashMapStorage::set(
    const Key& key,
    const CacheEntry& entry
)
{
    storage_[key] = entry;
    return Status::SUCCESS;
}

const CacheEntry* HashMapStorage::find(
    const Key& key
) const
{
    auto it = storage_.find(key);

    if(it == storage_.end())
    {
        return nullptr;
    }

    return &(it->second);
}

Status HashMapStorage::erase(
    const Key& key
)
{
    auto it = storage_.find(key);

    if(it == storage_.end())
    {
        return Status::KEY_NOT_FOUND;
    }

    storage_.erase(it);

    return Status::SUCCESS;
}

bool HashMapStorage::exists(
    const Key& key
) const
{
    return storage_.find(key) != storage_.end();
}

void HashMapStorage::clear()
{
    storage_.clear();
}

size_t HashMapStorage::size() const
{
    return storage_.size();
}

}