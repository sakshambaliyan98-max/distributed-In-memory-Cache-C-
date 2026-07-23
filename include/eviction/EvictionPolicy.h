#pragma once

#include <optional>

#include "common/Types.h"

namespace cache
{

class EvictionPolicy
{
public:

    virtual ~EvictionPolicy() = default;

    virtual void onInsert(const Key& key) = 0;

    virtual void onAccess(const Key& key) = 0;

    virtual void onErase(const Key& key) = 0;

    virtual std::optional<Key> evict() = 0;

    virtual void clear() = 0;
};

}