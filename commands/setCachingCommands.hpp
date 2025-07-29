#ifndef SET_ADD_REMOVE_MEMBER_COMMANDS_HPP
#define SET_ADD_REMOVE_MEMBER_COMMANDS_HPP

#include "../store/RedisDataStore.hpp"
#include "ICachingCommands.hpp"

class setAddCommand : public ICachingCommands {
    public : 
        static unique_ptr<ICachingCommands> create() {
            return make_unique<setAddCommand> ();
        }

        string execute(class RedisDataStore& store, const vector<string> &args) override {
            if (args.size() < 3) return "-ERR wrong number of arguments for 'SADD'";

            int added = store.sAdd(args[1], vector<string>(args.begin()+2, args.end()));
            return ":" + to_string(added) + "";
        }
};

class setRemCommand : public ICachingCommands {
    public: 
        static unique_ptr<ICachingCommands> create() {
            return make_unique<setRemCommand> ();
        }

        string execute(class RedisDataStore& store, const vector<string> &args) override {
            if (args.size() < 3) return "-ERR wrong number of arguments for 'SREM'";

            int removed = store.sRem(args[1], vector<string>(args.begin()+2, args.end()));
            return ":" + to_string(removed) + "";
        }
};

class setMemberCommand : public ICachingCommands {
public:
    
     static unique_ptr<ICachingCommands> create() { return make_unique<setMemberCommand>(); }

    string execute(class RedisDataStore& store, const vector<string> &args) override {
            if (args.size() != 2) return "-ERR wrong number of arguments for'SMEMBERS'";
            auto members = store.sMember(args[1]);
            string resp = "*" + to_string(members.size()) + "";
            for (auto &m : members) {
                resp += "$" + to_string(m.size()) + "" + m + "";
            }
            return resp;
    }
};

#endif