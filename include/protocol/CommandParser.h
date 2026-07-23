#pragma once

#include <optional>
#include <string>

#include "protocol/Command.h"
using namespace std;
namespace cache
{

// Parses the simple whitespace-delimited text protocol described in the
//   SET user Saksham
//   GET user
//   DELETE user
//   EXISTS user

class CommandParser
{
public:

    static optional<Command> parse(const string& line);
};

}
