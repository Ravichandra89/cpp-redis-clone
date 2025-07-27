// PublishCommand.hpp
#ifndef PUBLISH_COMMAND_HPP
#define PUBLISH_COMMAND_HPP

#include "ISocketCommand.hpp"
#include "../manager/PubSub.hpp"

using namespace std;

class PublishCommand : public ISocketCommand
{
public:
    static unique_ptr<ICommand> create()
    {
        return make_unique<PublishCommand>();
    }

    void execute(int clientSock, const vector<string> &args) override
    {
        if (args.size() < 3)
        {
            string err = "-ERR usage: PUBLISH <channel> <message>\r\n";
            send(clientSock, err.c_str(), err.size(), 0);
            return;
        }

        const string &channel = args[1];
        const string &message = args[2];
        PubSubManager::instance().publish(channel, message);

        string resp = "+PUBLISHED to " + channel + "\r\n";
        send(clientSock, resp.c_str(), resp.size(), 0);
    }
};

#endif 
