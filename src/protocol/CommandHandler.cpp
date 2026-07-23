#include "protocol/CommandHandler.h"

#include "protocol/CommandParser.h"

using namespace std;
namespace cache
{

CommandHandler::CommandHandler(CacheEngine& engine)
    : engine_(engine)
{
}

string CommandHandler::handleSet(const Command& command)
{
    // SET key value
    // SET key value EX <seconds>
    if(command.args.size() != 2 && command.args.size() != 4)
    {
        return "ERROR wrong number of arguments for 'SET'";
    }

    const string& key = command.args[0];
    const string& value = command.args[1];

    if(command.args.size() == 2)
    {
        engine_.set(key, value);
        return "OK";
    }

    if(command.args[2] != "EX" && command.args[2] != "ex")
    {
        return "ERROR expected 'EX <seconds>' after value";
    }

    try
    {
        long long seconds = stoll(command.args[3]);

        if(seconds <= 0)
        {
            return "ERROR TTL must be a positive number of seconds";
        }

        engine_.set(key, value, chrono::seconds(seconds));

        return "OK";
    }
    catch(const exception&)
    {
        return "ERROR TTL must be a positive integer";
    }
}

string CommandHandler::handleGet(const Command& command)
{
    if(command.args.size() != 1)
    {
        return "ERROR wrong number of arguments for 'GET'";
    }

    auto value = engine_.get(command.args[0]);

    if(!value.has_value())
    {
        return "(nil)";
    }

    return *value;
}

string CommandHandler::handleDelete(const Command& command)
{
    if(command.args.size() != 1)
    {
        return "ERROR wrong number of arguments for 'DELETE'";
    }

    Status status = engine_.erase(command.args[0]);

    if(status == Status::KEY_NOT_FOUND)
    {
        return "ERROR key not found";
    }

    return "OK";
}

string CommandHandler::handleExists(const Command& command)
{
    if(command.args.size() != 1)
    {
        return "ERROR wrong number of arguments for 'EXISTS'";
    }

    return engine_.exists(command.args[0]) ? "1" : "0";
}

string CommandHandler::handleStats(const Command& command)
{
    if(!command.args.empty())
    {
        return "ERROR wrong number of arguments for 'STATS'";
    }

    CacheStats stats = engine_.getStats();

    return "hits=" + to_string(stats.getHits())
        + " misses=" + to_string(stats.getMisses())
        + " size=" + to_string(engine_.size());
}

string CommandHandler::handlePing(const Command& command)
{
    if(!command.args.empty())
    {
        return "ERROR wrong number of arguments for 'PING'";
    }

    return "PONG";
}

string CommandHandler::execute(const string& rawLine)
{
    auto command = CommandParser::parse(rawLine);

    if(!command.has_value())
    {
        return "ERROR empty command";
    }

    const string& name = command->name;

    if(name == "SET")
    {
        return handleSet(*command);
    }

    if(name == "GET")
    {
        return handleGet(*command);
    }

    if(name == "DELETE" || name == "DEL")
    {
        return handleDelete(*command);
    }

    if(name == "EXISTS")
    {
        return handleExists(*command);
    }

    if(name == "STATS")
    {
        return handleStats(*command);
    }

    if(name == "PING")
    {
        return handlePing(*command);
    }

    if(name == "QUIT")
    {
        return "OK";
    }

    return "ERROR unknown command '" + name + "'";
}

}
