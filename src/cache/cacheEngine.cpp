#include "cache/cacheEngine.h"
#include "cache/cacheEntry.h"
#include "storage/HashMapStorage.h"
#include "eviction/LRUEvictionPolicy.h"

using namespace std;

namespace cache
{

CacheEngine::CacheEngine()
    : config_(),
      storage_(make_unique<HashMapStorage>()),
      evictionPolicy_(make_unique<LRUEvictionPolicy>())
{
}

CacheEngine::CacheEngine(
    const CacheConfig& config,
    unique_ptr<IStorage> storage,
    unique_ptr<EvictionPolicy> evictionPolicy
)
    : config_(config),
      storage_(move(storage)),
      evictionPolicy_(move(evictionPolicy))
{
}

const CacheStats& CacheEngine::getStats() const
{
    return stats_;
}

Status CacheEngine::set(
    const Key& key,
    const Value& value
)
{
    CacheEntry entry(value);

    if(storage_->exists(key))
    {
        storage_->set(key, entry);
        evictionPolicy_->onAccess(key);
        return Status::SUCCESS;
    }

    if(storage_->size() >= config_.getCapacity())
    {
        auto victim = evictionPolicy_->evict();

        if(victim.has_value())
        {
            storage_->erase(*victim);
        }
    }

    storage_->set(key, entry);
    evictionPolicy_->onInsert(key);

    return Status::SUCCESS;
}

Status CacheEngine::set(
    const Key& key,
    const Value& value,
    chrono::seconds ttl
)
{
    CacheEntry entry(value, ttl);

    if(storage_->exists(key))
    {
        storage_->set(key, entry);
        evictionPolicy_->onAccess(key);
        return Status::SUCCESS;
    }

    if(storage_->size() >= config_.getCapacity())
    {
        auto victim = evictionPolicy_->evict();

        if(victim.has_value())
        {
            storage_->erase(*victim);
        }
    }

    storage_->set(key, entry);
    evictionPolicy_->onInsert(key);

    return Status::SUCCESS;
}

optional<Value> CacheEngine::get(
    const Key& key
)
{
    auto entry = storage_->find(key);

    if(entry == nullptr)
    {
        stats_.recordMiss();
        return nullopt;
    }

    if(entry->hasExpired())
    {
        storage_->erase(key);
        evictionPolicy_->onErase(key);

        stats_.recordMiss();
        return nullopt;
    }

    evictionPolicy_->onAccess(key);
    stats_.recordHit();

    return entry->getValue();
}

Status CacheEngine::erase(
    const Key& key
)
{
    if(!storage_->exists(key))
        return Status::KEY_NOT_FOUND;

    storage_->erase(key);
    evictionPolicy_->onErase(key);

    return Status::SUCCESS;
}

bool CacheEngine::exists(
    const Key& key
) const
{
    auto entry = storage_->find(key);

    if(entry == nullptr)
        return false;

    return !entry->hasExpired();
}

size_t CacheEngine::size() const
{
    return storage_->size();
}

void CacheEngine::clear()
{
    storage_->clear();
    evictionPolicy_->clear();
}

}