#include<bits/stdc++.h>
#include<cassert>
using namespace std;

#include "cache/CacheEngine.h"

using namespace cache;

int main()
{
    CacheEngine cache;

    assert(cache.size() == 0);

    assert(cache.set("A","10") == Status::SUCCESS);

    assert(cache.exists("A"));

    auto value = cache.get("A");

    assert(value.has_value());

    assert(value.value() == "10");

    assert(cache.erase("A") == Status::SUCCESS);

    assert(!cache.exists("A"));

    assert(cache.getStats().getHits() == 1);

    assert(cache.getStats().getMisses() == 0);

    cout << "All tests passed\n";
}