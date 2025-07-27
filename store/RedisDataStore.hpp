#ifndef REDIS_CACHING_DATA_STORAGE_HPP
#define REDIS_CACHING_DATA_STORAGE_HPP

/**
 * Class Responsible for Redis In-memory data storage supporting 
 *      - Supporting String, List, Set, and TTL 
 *      - Methods include : LPush, RPush, lPop, RPop, Get, Set and etc...
 */

#include <string>
#include <set>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;

class RedisDataStore {
        // Data Containers for each data type <key, value> pair
        unordered_map<string, string> strings_;
        unordered_map<string, deque<string>> lists_;
        unordered_map<string, unordered_map<string, string>> hashes_;
        unordered_map<string, unordered_set<string>> sets_;
        unordered_map<string, chrono::time_point<chrono::steady_clock>> expiries_;

        bool isExpired(const string& key) {
            // check if expiries or not - found key, erase all data
            auto it = expiries_.find(key);
            if (it == expiries_.end()) return false;
            if (chrono::steady_clock::now() > it->second) {
                strings_.erase(key);
                lists_.erase(key);
                hashes_.erase(key);
                sets_.erase(key);
                expiries_.erase(key);
                return true;
            }
            return false;
        }

    public: 
        // String Operations - GET, SET
        void set(const string& key, const string& value) {
            strings_[key] = value;
            expiries_.erase(key);
        }

        optional<string> get(const string& key) {
            // return value for this key
            if (isExpired(key)) return {};
            auto it = strings_.find(key);
            if (it == strings_.end()) return {};
            return it->second;
        }

        // String Increment
        optional<long long> incr(const string& key) {
            auto opt = get(key);
            long long val = opt ? stoll(*opt) : 0;
            val++;
            set(key, to_string(val));
            return val;
        }

        optional<long long> decr(const string& key) {
            auto opt = get(key);
            long long val = opt ? stoll(*opt) : 0;
            val--;
            set(key, to_string(val));
            return val;
        }

        // List Operations LPush, RPush, lPop, rPop, 
        void lPush(const string& key, const string& val) {
            lists_[key].push_front(val);
            expiries_.erase(key);
        }

        void rPush(const string& key, const string& val) {
            lists_[key].push_back(val);
            expiries_.erase(key);
        }

        optional<string> lPop(const string& key) {
            // Removing from the list_head
            auto &dq = lists_[key];
            if (!dq.empty()) return {};
            string v = move(dq.front());
            dq.pop_front();
            return v;
        }

        optional<string> rPop(const string & key) {
            // Removing from the list_back
            auto & dq = lists_[key];
            if (!dq.empty()) return {};
            string v = move(dq.back());
            dq.pop_back();
            return v;
        }

        size_t llen(const string& key) {
            return lists_[key].size();
        }

        vector<string> lrange(const string& key, int start, int stop) {
            vector<string> out;
            auto &dq = lists_[key];
            int size = dq.size();
            if (start < 0) start = size + start;
            if (stop < 0) stop = size + stop;
            start = max(0, start);
            stop = min(stop, size - 1);
            for (int i=start; i<=stop; i++) {
                out.push_back(dq[i]);
            }
            return out;
        }

        // Hash Operations - hSet
        bool hSet(const string& key, const string& field, const string& value) {
            expiries_.erase(key);
            hashes_[key][field] = value;
            return true;
        }

        // Hash Operations - hGet
        optional<string> hget(const string& key, const string& field) {
            auto & mp = hashes_[key];
            auto it = mp.find(field);
            if (it == mp.end()) return {};
            return it->second;
        }

        // Set Operations - sAdd, sRem, sMembers
        int sAdd(const string& key, const vector<string> &members) {
            int count = 0;
            auto & st = sets_[key];
            for (auto & it : members) {
                if (st.insert(it).second) count++;
            }
            expiries_.erase(key);
            return count;
        }

        int sRem(const string& key, const vector<string> &members) {
            int count = 0;
            auto & st = sets_[key];
            for (auto & it : members) {
                if (st.erase(it)) count++;
            }
            return count;
        }

        vector<string> sMember(const string& key) {
            vector<string> out;
            for (auto & it : sets_[key]) out.push_back(it);
            return out;
        }

        // TTL + Expiries
        bool expire(const string& key, int seconds) {
            expiries_[key] = chrono::steady_clock::now() + chrono::seconds(seconds);
            return true;
        }

        int ttl(const string &key) {
            auto it = expiries_.find(key);
            if (it == expiries_.end()) return -1;             
            auto diff = chrono::duration_cast<chrono::seconds>(it->second - chrono::steady_clock::now()).count();
            return diff > 0 ? diff : -2;                     
        }
};

#endif