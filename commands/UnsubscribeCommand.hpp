// UnsubscribeCommand.hpp
#ifndef UNSUBSCRIBE_COMMAND_HPP
#define UNSUBSCRIBE_COMMAND_HPP

#include "ISocketCommand.hpp"
#include "../manager/PubSub.hpp"
using namespace std;

class UnsubscribeCommand : public ISocketCommand {
public:
    static unique_ptr<ICommand> create() {
        return make_unique<UnsubscribeCommand>();
    }

    void execute(int clientSock, const vector<string> &args) override {
        if (args.size() < 2) {
            string err = "-ERR missing channel name\r\n";
            send(clientSock, err.c_str(), err.size(), 0);
            return;
        }

        const string &channel = args[1];
        PubSubManager::instance().unsubscribe(channel, clientSock);

        string resp = "+UNSUBSCRIBED " + channel + "\r\n";
        send(clientSock, resp.c_str(), resp.size(), 0);
    }
};

#endif // UNSUBSCRIBE_COMMAND_HPP
