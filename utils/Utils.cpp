#include "IUtils.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace utils
{
    string trim(const string &s)
    {
        size_t start = 0;
        while (start < s.size() && isspace(static_cast<unsigned char>(s[start])))
            start++;
        if (start == s.size())
            return "";
        size_t end = s.size() - 1;
        while (end > start && isspace(static_cast<unsigned char>(s[end])))
            end--;
        return s.substr(start, end - start + 1);
    }

    vector<string> split(const string & line) {
        vector<string> tokens;
        istringstream iss(line);
        string tok;
        while (iss >> tok) {
            tokens.push_back(tok);
        }
        return tokens;
    }
}