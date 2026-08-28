#include <csignal>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "cache/cacheConfig.h"
#include "cache/cacheEngine.h"
#include "eviction/LRUEvictionPolicy.h"
#include "network/TcpServer.h"
#include "protocol/CommandHandler.h"
#include "storage/HashMapStorage.h"

using namespace std;
namespace
{
    constexpr uint16_t kDefaultPort = 6380;

    cache::TcpServer* g_server = nullptr;

    void handleShutdownSignal(int)
    {
        if(g_server != nullptr)
        {
            g_server->stop();
        }
    }
}

int main(int argc, char* argv[])
{
    uint16_t port = kDefaultPort;

    if(argc > 1)
    {
        port = static_cast<uint16_t>(atoi(argv[1]));
    }

    size_t threadCount = thread::hardware_concurrency();

    if(threadCount == 0)
    {
        threadCount = 4;
    }

    cache::CacheConfig config;

    cache::CacheEngine engine(
        config,
        make_unique<cache::HashMapStorage>(),
        make_unique<cache::LRUEvictionPolicy>()
    );

    cache::CommandHandler commandHandler(engine);

    // TcpServer owns and manages its own internal ThreadPool -
    // it just needs to know how many worker threads to spin up.
    cache::TcpServer server(port, threadCount, commandHandler);

    if(!server.start())
    {
        cerr << "Failed to start server on port " << port << "\n";
        return EXIT_FAILURE;
    }

    g_server = &server;

    signal(SIGINT, handleShutdownSignal);
    signal(SIGTERM, handleShutdownSignal);

    cout << "Distributed Cache server started on port " << port
              << " with " << threadCount << " worker threads.\n"
              << "Connect with: nc localhost " << port << "\n";

    server.run();

    cout << "Server shutting down...\n";

    return EXIT_SUCCESS;
}
