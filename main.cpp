#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>

#include "commands/ICommand.hpp"
#include "commands/CommandFactory.hpp"
#include "store/HashTable.hpp"
#include "store/DefaultHashAlgorithm.hpp"
#include "utils/IUtils.hpp"
#include "store/IHashFunction.hpp"
#include "manager/PubSub.hpp"

using namespace std;

constexpr int DEFAULT_PORT = 6379;
constexpr int BACKLOG = 128;
constexpr int BUFFER_SIZE = 4096;
atomic<bool> keepRunning(true);

void handleClient(int clientSock, HashTable &store) {
    string leftover;
    CommandFactory factory;
    char buf[BUFFER_SIZE];

    while (keepRunning)
    {
        ssize_t bytes = recv(clientSock, buf, BUFFER_SIZE, 0);
        if (bytes > 0)
        {
            leftover.append(buf, static_cast<size_t>(bytes));

            size_t pos;
            while ((pos = leftover.find('\n')) != string::npos)
            {
                string line = leftover.substr(0, pos);
                leftover.erase(0, pos + 1);
                line = utils::trim(line);
                if (line.empty())
                    continue;

                auto tokens = utils::split(line);
                if (tokens.empty())
                    continue;

                string cmdName = tokens[0];
                transform(cmdName.begin(), cmdName.end(), cmdName.begin(), ::toupper);

                auto cmd = factory.create(cmdName);
                if (!cmd)
                {
                    send(clientSock, "-ERR unknown command\r\n", 25, 0);
                    continue;
                }

                // Check if it's a socket-based command
                if (auto socketCmd = dynamic_cast<ISocketCommand *>(cmd.get()))
                {
                    socketCmd->execute(clientSock, tokens);
                }
                else
                {
                    string response = cmd->execute(store, tokens);
                    send(clientSock, response.c_str(), response.size(), 0);
                }
            }
        }
        else if (bytes == 0)
        {
            break; // client closed connection
        }
        else
        {
            if (errno == EINTR)
                continue;
            break; // error
        }
    }

    PubSubManager::instance().removeClient(clientSock);
    close(clientSock);
}

void signalHandler(int) {
    keepRunning = false;
}

int main() {
    signal(SIGINT, signalHandler); 
    int listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0)
    {
        cerr << "socket() failed: " << strerror(errno) << "\n";
        return 1;
    }
    int opt = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(DEFAULT_PORT);
    if (::bind(listenSock, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        cerr << "bind() failed: " << strerror(errno) << "\n";
        return 1;
    }
    if (listen(listenSock, BACKLOG) < 0)
    {
        cerr << "listen() failed: " << strerror(errno) << "\n";
        return 1;
    }
    cout << "Redis-clone listening on port " << DEFAULT_PORT << "\n";
    HashTable store(make_unique<DefaultHash>());
    while (keepRunning)
    {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);
        int clientSock = accept(listenSock, (sockaddr *)&clientAddr, &len);
        if (clientSock < 0)
        {
            if (errno == EINTR)
                continue;
            cerr << "accept() failed: " << strerror(errno) << "\n";
            break;
        }
        thread(handleClient, clientSock, ref(store)).detach();
    }
    close(listenSock);
    cout << "Server shutting down\n";
    return 0;
}
