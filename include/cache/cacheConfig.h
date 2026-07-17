#pragma once

#include<bits/stdc++.h>

using namespace std;

#include "common/Constants.h"

namespace cache
{

class CacheConfig
{
private:

    size_t capacity_;

public:

    explicit CacheConfig(size_t capacity = DEFAULT_CAPACITY);

    size_t getCapacity() const;
};

}