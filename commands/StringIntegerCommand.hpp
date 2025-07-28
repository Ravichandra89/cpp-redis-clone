/**
 * INCR && DECR
 */

#ifndef INCR_DECR_COMMANDS_HPP
#define INCR_DECR_COMMANDS_HPP

#include "../store/RedisDataStore.hpp"
#include "ICachingCommands.hpp"

class IncrCommand : public ICachingCommands {
    public: 
        static unique_ptr<ICachingCommands> create() {
            return make_unique<IncrCommand>();
        }
        
        string execute(class RedisDataStore& store, const vector<string> &args) override {
            if (args.size() != 2) return "-ERR wrong number of arguments for 'INCR'";

            auto ans = store.incr(args[1]);
            if (!ans) return "-ERR value is not an integer or out of range";
             return ":" + to_string(*ans) + "";
        }

};

class DecrCommand : public ICachingCommands{

public: 
    static unique_ptr<ICachingCommands> create() {
            return make_unique<DecrCommand>();
        }
        
        string execute(class RedisDataStore& store, const vector<string> &args) override {
            if (args.size() != 2) return "-ERR wrong number of arguments for 'DECR'";

            auto ans = store.decr(args[1]);
            if (!ans) return "-ERR value is not an integer or out of range";
             return ":" + to_string(*ans) + "";
        }
};

#endif