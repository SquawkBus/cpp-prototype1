#ifndef SQUAWKBUS_CLIENT_UTILS_HPP
#define SQUAWKBUS_CLIENT_UTILS_HPP

#include <algorithm> 
#include <cstddef>
#include <cctype>
#include <locale>
#include <string>
#include <vector>

std::vector<std::string> split(std::string s, std::string delimiter) {
    std::vector<std::string> parts;

    size_t start = 0, end;
    while ((end = s.find(delimiter, start)) != std::string::npos)
    {
        auto token = s.substr(start, end - start);
        start = end + delimiter.length();
        parts.push_back (token);
    }

    parts.push_back (s.substr(start));

    return parts;
}

// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

// trim from start (copying)
inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

#endif // SQUAWKBUS_CLIENT_UTILS_HPP
