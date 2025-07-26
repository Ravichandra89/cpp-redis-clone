/**
 * Socket_Command_Adapter make Compatible to use Existing interface with new version using Adapter Design pattern

*/
#ifndef REDIS_IN_MEMORY_PUB_SUB_SOCKET_ADAPTER_HPP
#define REDIS_IN_MEMORY_PUB_SUB_SOCKET_ADAPTER_HPP

#include "ISocketCommand.hpp"
#include "ICommand.hpp"

class SocketAdapter : public ICommand {
        int clientSok;
        unique_ptr<ISocketCommand> socket_commands;

    public: 
        // Constructor - SocketAdapter
        SocketAdapter(unique_ptr<ISocketCommand> cmd, int sok) : socket_commands(move(cmd)), clientSok(sok) {};

        // Execute method - updated for Socket Interface
        string execute(HashTable& store, const vector<string> &args) override {
            socket_commands->execute(clientSok, args);
            return "";
        }
};

#endif