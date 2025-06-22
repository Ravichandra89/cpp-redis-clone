#ifndef REDIS_IN_MEMORY_UTILS_INTERFACE_HPP
#define REDIS_IN_MEMORY_UTILS_INTERFACE_HPP

#include <string>
#include <vector>
using namespace std;

namespace utils {
    string trim(const string & s);
    vector<string> split(const string & line);
}

#endif