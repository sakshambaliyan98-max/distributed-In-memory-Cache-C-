#include "cache/CacheEngine.h"

using namespace std;

namespace cache
{

CacheEngine::CacheEngine(const CacheConfig& config)
    : config_(config)
{
}

Status CacheEngine::set(const Key& key,
                        const value& value)
{
    if(key.empty())
        return Status::INVALID_KEY;

    storage_.insert_or_assign(key, CacheEntry(value));

    return Status::SUCCESS;
}

optional<value> CacheEngine::get(const Key& key)
{
    auto it = storage_.find(key);

    if(it == storage_.end())
    {
        stats_.recordMiss();
        return nullopt;
    }

    stats_.recordHit();

    return it->second.getValue();
}

Status CacheEngine::erase(const Key& key)
{
    if(storage_.erase(key) == 0)
        return Status::KEY_NOT_FOUND;

    return Status::SUCCESS;
}

bool CacheEngine::exists(const Key& key) const
{
    return storage_.count(key);
}

void CacheEngine::clear()
{
    storage_.clear();
}

size_t CacheEngine::size() const
{
    return storage_.size();
}

const CacheStats& CacheEngine::getStats() const
{
    return stats_;
}

}