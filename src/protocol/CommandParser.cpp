#include "protocol/CommandParser.h"

#include <algorithm>
#include <cctype>
#include <sstream>

using namespace std;
namespace cache
{

optional<Command> CommandParser::parse(const string& line)
{
    istringstream stream(line);

    vector<string> tokens;
    string token;

    while(stream >> token)
    {
        tokens.push_back(token);
    }

    if(tokens.empty())
    {
        return nullopt;
    }

    Command command;

    command.name = tokens.front();

    transform(
        command.name.begin(),
        command.name.end(),
        command.name.begin(),
        [](unsigned char c)
        {
            return static_cast<char>(toupper(c));
        }
    );

    command.args.assign(tokens.begin() + 1, tokens.end());

    return command;
}

}
