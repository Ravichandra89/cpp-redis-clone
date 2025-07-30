/**
 * TTL Eviction Strategy
 */

#ifndef TTL_EVICTION_POLICY
#define TTL_EVICTION_POLICY


#include "EvictionPolicy.hpp"
#include <map>
#include <string>
using namespace std::chrono;

class VolatileTTL : public EvictionPolicy {
        map<steady_clock::time_point, string> mp;

    public: 
        void markVolatile(const string& key, steady_clock::time_point when) {
            mp.emplace(when, key);
        }

        void recordAccess(const string& key) override {
            // Do nothing
        }

        bool isRemovable(const string& key) const override {
            return !mp.empty();
        }

        string selectEvictionKey() override {
            if (mp.empty()) return {};
            auto it = mp.begin();
            string target = it->second;
            mp.erase(it);
            return target;
        }
};

#endif