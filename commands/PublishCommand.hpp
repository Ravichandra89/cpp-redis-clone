/**
 * Socket PUBLISH - Command
 */
#ifndef REDIS_IN_MEMORY_PUB_SUB_SOCKET_PUBLISH_COMMAND_HPP
#define REDIS_IN_MEMORY_PUB_SUB_SOCKET_PUBLISH_COMMAND_HPP

#include "ISocketCommand.hpp"
#include "../manager/PubSub.hpp"
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

class PublishCommand : public ISocketCommand {
    public: 
        void execute(int clientSok, const vector<string> &args) override {
            // args[0] = "PUBLISH" && args[1] = channel_name  && args[2] = "Message_Part"

            if (args.size() < 3) {
                const char* err = "-ERR wrong number of arguments for 'PUBLISH'\r\n";
                send(clientSok, err, strlen(err), 0);
                return;
            }

            const auto& channel = args[1];
            string message;
            for (size_t t=2; t<args.size(); t++) {
                if (t > 2) message += ' ';
                message += args[t];
            }

            int count = PubSubManager::instance().publish(channel, message);
            string resp = ":" + std::to_string(count) + "\r\n";
            send(clientSok, resp.data(), resp.size(), 0);     // Send_Method
        }
};

#endif

