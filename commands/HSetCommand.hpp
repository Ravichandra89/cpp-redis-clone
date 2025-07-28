/**
 * Hash_Set command - HSet for Overwrites the hash value for particular Key. If Key doesn't exist then create new Key.
 */
#ifndef HSET_COMMAND_HPP
#define HSET_COMMAND_HPP

#include "../store/RedisDataStore.hpp";
#include "ICachingCommands.hpp"

class hSetCommand : public ICachingCommands
{
public:
    static unique_ptr<ICachingCommands> create()
    {
        return make_unique<hSetCommand>();
    }

    // args[1] == key && args[2] == field && args[3] == value

    string execute(class RedisDataStore &store, const vector<string> &args) override
    {
        if (args.size() < 4)
            return "-ERR wrong number of arguments for 'HSET'\r\n";
        bool added = store.hSet(args[1], args[2], args[3]);
        return ":" + to_string(added ? 1 : 0) + "\r\n";
    }
};

#endif