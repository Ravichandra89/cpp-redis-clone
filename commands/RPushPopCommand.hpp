/**
 * RPush && RPop Command - List back push and list back remove commands
 */
#ifndef RPUSH_RPOP_COMMANDS_HPP
#define RPUSH_RPOP_COMMANDS_HPP

#include "../store/RedisDataStore.hpp"
#include "ICachingCommands.hpp"

class rPush : public ICachingCommands {
    public: 
        static unique_ptr<ICachingCommands> create () {
            return make_unique<rPush> ();
        }

        string execute(class RedisDataStore& store, const vector<string> &args) override {
            if (args.size() < 3) return "-ERR wrong number of arguments for 'RPUSH'";

            for (size_t t=2; t < args.size(); t++) {
                store.rPush(args[1], args[t]);
            }

            return ":" + to_string(store.llen(args[1])) + " ";
        }
};

class rPop : public ICachingCommands {
    public: 
        static unique_ptr<ICachingCommands> create () {
            return make_unique<rPop> ();
        }

        string execute(class RedisDataStore& store, const vector<string> &args) override {
            if (args.size() != 2) return "-ERR wrong number of arguments for 'RPOP'";

            auto ans = store.rPop(args[1]);
            if (!ans) return "$-1";
            return "$" + to_string(ans->size()) + "" + *ans + "";
        }
};

#endif