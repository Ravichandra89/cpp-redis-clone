#ifndef EXPIRE_COMMAND_HPP
#define EXPIRE_COMMAND_HPP

#include "../store/RedisDataStore.hpp"
#include "ICachingCommands.hpp"

using namespace std;

class ExpireCommand : public ICachingCommands {
    public: 
        static unique_ptr<ICachingCommands> create() {
            return make_unique<ExpireCommand>();
        }

        string execute(class RedisDataStore& store, const vector<string> & args) override {
            if (args.size() != 3) return "-ERR wrong number of arguments for 'EXPIRE'";

            int sec = stoi(args[2]);
            bool temp = store.expire(args[1], sec);
            return ":" + to_string(temp ? 1 : 0) + "";
        }
};

class TtlCommand : public ICachingCommands {
    public: 
        static unique_ptr<ICachingCommands> create() {
            return make_unique<TtlCommand>();
        }

        string execute(class RedisDataStore& store, const vector<string> & args) override {
            if (args.size() != 2) return "-ERR Wrong number of arguments for 'TTL'";

            int ttl = store.ttl(args[1]);
            return ":" + to_string(ttl)+"";
        }
};

#endif