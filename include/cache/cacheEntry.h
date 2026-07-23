#pragma once

#include<bits/stdc++.h>

using namespace std;

namespace cache
{

class CacheEntry
{
private:

    string value_;

    optional<chrono::steady_clock::time_point> expiryTime_;

public:

    CacheEntry();

    explicit CacheEntry(const string& value);

    CacheEntry(
        const string& value,
        chrono::seconds ttl
    );

    const string& getValue() const;

    void setValue(const string& value);

    void setTTL(chrono::seconds ttl);

    void clearTTL();

    bool hasExpired() const;
};

}