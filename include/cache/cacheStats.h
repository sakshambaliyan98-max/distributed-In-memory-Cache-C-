#pragma once

#include<bits/stdc++.h>

using namespace std;

namespace cache
{

class CacheStats
{
private:

    uint64_t hits_;

    uint64_t misses_;

public:

    CacheStats();

    void recordHit();

    void recordMiss();

    uint64_t getHits() const;

    uint64_t getMisses() const;
};

}