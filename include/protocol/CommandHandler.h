#pragma once

#include <string>

#include "cache/cacheEngine.h"
#include "protocol/Command.h"

using namespace std;

namespace cache
{

// Executes parsed commands against a CacheEngine and renders a
// text response line. CacheEngine is thread-safe, so a single
// CommandHandler instance can be shared by every client-handling
// worker thread.
class CommandHandler
{
private:

    CacheEngine& engine_;

    string handleSet(const Command& command);

    string handleGet(const Command& command);

    string handleDelete(const Command& command);

    string handleExists(const Command& command);

    string handleStats(const Command& command);

    string handlePing(const Command& command);

public:

    explicit CommandHandler(CacheEngine& engine);

    // Parses and executes a single raw request line, returning the
    // response line to send back to the client (no trailing newline).
    string execute(const string& rawLine);
};

}
