/**
 * Randomaly Deleting the keys when they on eviction
 * 
 */

#ifndef ALL_KEYS_RANDOM_EVICTION_HPP
#define ALL_KEYS_RANDOM_EVICTION_HPP

#include "EvictionPolicy.hpp"
#include <unordered_map>
#include <unordered_set>


class AllKeysRandom : public EvictionPolicy {
        unordered_set<string> keys;

    public: 
        void recordAccess(const string& key) override {
            keys.insert(key);
        }

        bool isRemovable(const string& key) const override {
            return !key.empty();
        }

        string selectEvictionKey() override {
            if (keys.empty()) return {};
            // Randomally choose any key for eviction
            int selected = rand() % keys.size();
            auto it = keys.begin();
            while (selected--) {
                it++;
            }
            string target = *it;
            keys.erase(it);
            return target;
        }
};

#endif