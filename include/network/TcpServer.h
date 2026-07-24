#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <atomic>
#include <thread>

#include "concurrency/ThreadPool.h"
#include "protocol/CommandHandler.h"

namespace cache
{

class TcpServer
{
private:

    SOCKET serverSocket_;

    int port_;

    std::atomic<bool> running_;

    ThreadPool threadPool_;

    CommandHandler& handler_;

    std::thread acceptThread_;

    void acceptLoop();

    void handleClient(SOCKET clientSocket);

public:

    TcpServer(
        int port,
        size_t threadCount,
        CommandHandler& handler
    );

    ~TcpServer();

    bool start();

    void run();

    void stop();
};

}