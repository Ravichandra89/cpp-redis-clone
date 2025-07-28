/**
 * LPOP Command : Removing element from the list Head
 */
#ifndef REDIS_IN_MEMORY_CACHING_LPOP_COMMAND_HPP
#define REDIS_IN_MEMORY_CACHING_LPOP_COMMAND_HPP

#include "../store/RedisDataStore.hpp"
#include "ICachingCommands.hpp"

class lPopCommand : public ICachingCommands {
    public:
        static unique_ptr<ICachingCommands> create() {
            return make_unique<lPopCommand>();
        }

        string execute(class RedisDataStore& store,const vector<string> &args) override {
            if (args.size() != 2) return "-ERR wrong number of arguments for 'LPOP'\r\n";

            auto temp = store.lPop(args[1]);
            if (!temp) return "$-1\r\n";
            return "$" + to_string(temp->size()) + "\r\n" + *temp + "\r\n";
        }
};

#endif