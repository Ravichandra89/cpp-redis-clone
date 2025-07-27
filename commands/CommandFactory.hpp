// CommandFactory.hpp
#ifndef COMMAND_FACTORY_HPP
#define COMMAND_FACTORY_HPP

#include <unordered_map>
#include <functional>
#include <memory>
#include <string>
#include <algorithm>

#include "ICommand.hpp"
#include "SetCommand.hpp"
#include "GetCommand.hpp"
#include "DelCommand.hpp"

// Socket Commands
#include "SubscribeCommand.hpp"
#include "UnsubscribeCommand.hpp"
#include "PublishCommand.hpp"

class CommandFactory
{
    using Creator = function<unique_ptr<ICommand>()>;

    unordered_map<string, Creator> registry;

public:
    CommandFactory()
    {
        registry["SET"] = &SetCommand::create;
        registry["GET"] = &GetCommand::create;
        registry["DEL"] = &DelCommand::create;

        // Register Socket Commands
        registry["SUBSCRIBE"] = &SubscribeCommand::create;
        registry["UNSUBSCRIBE"] = &UnsubscribeCommand::create;
        registry["PUBLISH"] = &PublishCommand::create;
    }

    unique_ptr<ICommand> create(const string &name) const
    {
        string upperName = name;
        transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);

        auto it = registry.find(upperName);
        if (it != registry.end())
        {
            return (it->second)();
        }

        return nullptr;
    }
};

#endif
