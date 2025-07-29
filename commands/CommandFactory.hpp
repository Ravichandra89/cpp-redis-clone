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

// Additional Commands for Redis
#include "LPushCommand.hpp"
#include "LPopCommand.hpp"
#include "RPushPopCommand.hpp"
#include "LlenCommand.hpp"
#include "setCachingCommands.hpp"
#include "HGetCommand.hpp"
#include "HSetCommand.hpp"
#include "StringIntegerCommand.hpp"
#include "ExpireCommand.hpp"

#include "CachingAdapter.hpp"

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

        // Additional Commands Creation
        registry["LPUSH"] = []() {
            return make_unique<CachingAdapter>(lPushCommand::create());
        };

        registry["LPOP"] = []() {
            return make_unique<CachingAdapter> (lPopCommand::create());
        };

        registry["RPUSH"] = []() {
            return make_unique<CachingAdapter> (rPush::create());
        };

        registry["RPOP"] = []() {
            return make_unique<CachingAdapter> (rPop::create());
        };

        registry["HSET"] = []() {
            return make_unique<CachingAdapter> (hSetCommand::create());
        };

        registry["HGET"] = []() {
            return make_unique<CachingAdapter> (hGetCommand::create());
        };

        registry["SADD"] = []() {
            return make_unique<CachingAdapter> (setAddCommand::create());
        };

        registry["SREM"] = []() {
            return make_unique<CachingAdapter> (setRemCommand::create());
        };

        registry["INCR"] = []() {
            return make_unique<CachingAdapter> (IncrCommand::create());
        };

        registry["DECR"] = []() {
            return make_unique<CachingAdapter> (DecrCommand::create());
        };

        registry["SMEMBER"] = []() {
            return make_unique<CachingAdapter> (setMemberCommand::create());
        };

        registry["LLEN"] = []() {
            return make_unique<CachingAdapter> (lLenCommand::create());
        };

        registry["LRANGE"] = []() {
            return make_unique<CachingAdapter> (lRangeCommand::create());
        };

        registry["EXPIRE"] = []() {
            return make_unique<CachingAdapter> (ExpireCommand::create());
        };

        registry["TTL"] = []() {
            return make_unique<CachingAdapter> (TtlCommand::create());
        };
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
