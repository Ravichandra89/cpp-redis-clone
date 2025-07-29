/**
 * Interface for Eviction_Policy
 */
#ifndef EVICTION_POLICY_HPP
#define EVICTION_POLICY_HPP

#include <string>
using namespace std;

class EvictionPolicy {
    public:     
        virtual ~EvictionPolicy() = default;
        // Method : recordAccess, isRemovale(), SelectEvictKey()

        virtual void recordAccess(const string& key) = 0;
        virtual bool isRemovable(const string& key) const = 0;
        virtual string selectEvictionKey() = 0;

};

#endif