/**
 * HGET Command : Return the value associated with field
 */

#ifndef HGET_COMMAND_HPP
#define HGET_COMMAND_HPP

#include "../store/RedisDataStore.hpp"
#include "ICachingCommands.hpp"

class hGetCommand : public ICachingCommands {
    public: 
        static unique_ptr<ICachingCommands> create() {
            return make_unique<hGetCommand> ();
        }

        string execute(class RedisDataStore& store, const vector<string>& args) override {
            if (args.size() != 3) return "-ERR wrong number of arguments for 'HGET'\r\n";
            auto ans = store.hget(args[1], args[2]);
            if (!ans)  return "$-1\r\n";
            return "$" + to_string(ans->size()) + "\r\n" + *ans + "\r\n";
        }
};

#endif