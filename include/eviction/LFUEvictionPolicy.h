#pragma once

#include "eviction/EvictionPolicy.h"

namespace cache
{

class LFUEvictionPolicy : public EvictionPolicy
{
public:

    void onInsert(const Key&) override {}
    void onAccess(const Key&) override {}
    void onErase(const Key&) override {}

    std::optional<Key> evict() override
    {
        return std::nullopt;
    }

    void clear() override {}
};

}