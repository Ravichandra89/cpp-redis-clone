/**
 * Adapter Pattern Interface for Socket Commands : PUBLISH, SUBSCRIBE, UNSUBSCRIBE
 */
#ifndef REDIS_IN_MEMORY_PUB_SUB_SOCKET_INTERFACE_HPP
#define REDIS_IN_MEMORY_PUB_SUB_SOCKET_INTERFACE_HPP

#include <string>
#include <vector>
using namespace std;

class ISocketCommand {
    public: 
        virtual ~ISocketCommand() = default;
        virtual void execute(int clientSok, const vector<string> &args) = 0;
};

#endif