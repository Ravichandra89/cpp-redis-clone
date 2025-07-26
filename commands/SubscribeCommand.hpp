#ifndef REDIS_IN_MEMORY_PUB_SUB_SOCKET_SUBSCRIBE_COMMAND_HPP
#define REDIS_IN_MEMORY_PUB_SUB_SOCKET_SUBSCRIBE_COMMAND_HPP

#include "ISocketCommand.hpp"
#include "SocketAdapter.hpp"
#include "../manager/PubSub.hpp"

#include <string>
#include <vector>
#include <sys/socket.h> // for send()
#include <sstream>      // in case to_string fallback needed

using namespace std;

class SubscribeCommand : public ISocketCommand
{
public:
    void execute(int clientSock, const vector<string> &args) override
    {
        if (args.size() < 2)
        {
            string error = "-ERR wrong number of arguments for 'subscribe' command\r\n";
            send(clientSock, error.c_str(), error.size(), 0);
            return;
        }

        for (size_t i = 1; i < args.size(); i++)
        {
            const string &channel = args[i];

            // Subscribe client socket to the channel
            PubSubManager::instance().subscribe(channel, clientSock);

            // Get current count of subscribers to this channel
            int count = PubSubManager::instance().subscriberCount(channel);

            // Construct RESP response
            string resp = "*3\r\n";
            resp += "$9\r\nsubscribe\r\n";
            resp += "$" + to_string(channel.size()) + "\r\n" + channel + "\r\n";
            resp += ":" + to_string(count) + "\r\n";

            send(clientSock, resp.c_str(), resp.size(), 0);
        }
    }
};

#endif
