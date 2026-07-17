#pragma once

#include<bits/stdc++.h>

using namespace std;

namespace cache
{

class CacheEntry
{
private:

    string value_;

public:

    CacheEntry();

    explicit CacheEntry(const string& value);

    const string& getValue() const;

    void setValue(const string& value);
};

}