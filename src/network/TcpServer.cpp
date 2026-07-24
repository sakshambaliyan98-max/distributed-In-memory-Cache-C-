#include "network/TcpServer.h"

#include <iostream>
#include <string>

using namespace std;

namespace cache
{

    TcpServer::TcpServer(
        int port,
        size_t threadCount,
        CommandHandler &handler)
        : serverSocket_(INVALID_SOCKET),
          port_(port),
          running_(false),
          threadPool_(threadCount),
          handler_(handler)
    {
    }

    TcpServer::~TcpServer()
    {
        stop();
    }

    bool TcpServer::start()
    {
        if (running_.load())
        {
            return true;
        }

        WSADATA wsaData;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            cerr << "WSAStartup failed\n";
            return false;
        }

        serverSocket_ = socket(
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP);

        if (serverSocket_ == INVALID_SOCKET)
        {
            cerr << "socket() failed: "
                 << WSAGetLastError()
                 << '\n';

            WSACleanup();
            return false;
        }

        BOOL reuse = TRUE;

        setsockopt(
            serverSocket_,
            SOL_SOCKET,
            SO_REUSEADDR,
            reinterpret_cast<const char *>(&reuse),
            sizeof(reuse));

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(static_cast<u_short>(port_));

        if (bind(
                serverSocket_,
                reinterpret_cast<sockaddr *>(&address),
                sizeof(address)) == SOCKET_ERROR)
        {
            cerr << "bind() failed: "
                 << WSAGetLastError()
                 << '\n';

            closesocket(serverSocket_);
            WSACleanup();

            return false;
        }

        if (listen(serverSocket_, SOMAXCONN) == SOCKET_ERROR)
        {
            cerr << "listen() failed: "
                 << WSAGetLastError()
                 << '\n';

            closesocket(serverSocket_);
            WSACleanup();

            return false;
        }

        running_.store(true);

        acceptThread_ = thread(
            &TcpServer::acceptLoop,
            this);

        cout
            << "Server listening on port "
            << port_
            << '\n';

        return true;
    }
    void TcpServer::acceptLoop()
    {
        while (running_.load())
        {
            sockaddr_in clientAddr{};
            int clientLen = sizeof(clientAddr);

            SOCKET clientSocket = accept(
                serverSocket_,
                reinterpret_cast<sockaddr *>(&clientAddr),
                &clientLen);

            if (clientSocket == INVALID_SOCKET)
            {
                if (running_.load())
                {
                    cerr
                        << "accept() failed: "
                        << WSAGetLastError()
                        << '\n';
                }

                continue;
            }

            threadPool_.enqueue(
                [this, clientSocket]()
                {
                    handleClient(clientSocket);
                });
        }
    }
    void TcpServer::handleClient(SOCKET clientSocket)
    {
        constexpr int BUFFER_SIZE = 4096;

        char buffer[BUFFER_SIZE];

        string pending;

        while (running_.load())
        {
            int received = recv(
                clientSocket,
                buffer,
                BUFFER_SIZE,
                0);

            if (received <= 0)
            {
                break;
            }

            pending.append(
                buffer,
                received);

            size_t pos;

            while ((pos = pending.find('\n')) != string::npos)
            {
                string command = pending.substr(
                    0,
                    pos);

                pending.erase(
                    0,
                    pos + 1);

                if (!command.empty() &&
                    command.back() == '\r')
                {
                    command.pop_back();
                }

                if (command.empty())
                {
                    continue;
                }

                string response =
                    handler_.execute(command);

                response += "\r\n";

                send(
                    clientSocket,
                    response.c_str(),
                    static_cast<int>(response.size()),
                    0);

                if (command == "QUIT")
                {
                    shutdown(
                        clientSocket,
                        SD_BOTH);

                    closesocket(clientSocket);

                    return;
                }
            }
        }

        shutdown(
            clientSocket,
            SD_BOTH);

        closesocket(clientSocket);
    }
    void TcpServer::run()
    {
        if (acceptThread_.joinable())
        {
            acceptThread_.join();
        }
    }

    void TcpServer::stop()
    {
        if (!running_.exchange(false))
        {
            return;
        }

        if (serverSocket_ != INVALID_SOCKET)
        {
            shutdown(
                serverSocket_,
                SD_BOTH);

            closesocket(serverSocket_);

            serverSocket_ =
                INVALID_SOCKET;
        }

        if (acceptThread_.joinable())
        {
            acceptThread_.join();
        }

        threadPool_.shutdown();

        WSACleanup();

        cout
            << "Server stopped.\n";
    }

}