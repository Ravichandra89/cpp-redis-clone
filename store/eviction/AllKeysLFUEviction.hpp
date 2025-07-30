/**
 * Least Frequently Used - Managing the count of record and based on this *reomove the data in eviction process.
 * 
 * Multiset : Can store multiple element with same value
 */

#ifndef ALL_KEYS_LFU_EVICTIOIN_HPP
#define ALL_KEYS_LFU_EVICTIOIN_HPP

#include "EvictionPolicy.hpp"
#include <string>
#include <unordered_map>
#include <set>

class AllKeysLFU : public EvictionPolicy {
        // Frequency Count <"Key", count> 
        unordered_map<string, int> freq;
        multiset<pair<int, string>> tree;

    public: 
        void recordAccess(const string& key) override {
            // Updating the frequency count on this function call
            int prev = freq[key];
            if (prev > 0) tree.erase(tree.find({prev, key}));
            int curr = prev + 1;
            freq[key] = curr;
            tree.insert({curr, key});
        }

        bool isRemovable(const string& key) const override {
            return true;
        }

        string selectEvictionKey() override {
            if (tree.empty()) return "";
            auto it = tree.begin();
            string target = it->second;
            tree.erase(it);
            freq.erase(target);

            // Return the eviction key
            return target;
        }
};

#endif