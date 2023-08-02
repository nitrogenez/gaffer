#pragma once

#include <gaffer.hh>

#include <string>


namespace gaffer::util::str {
    inline static std::string rtrim(const std::string& str, const std::string& chars = " ") {
        auto start = str.find_first_not_of(chars);

        return (start == std::string::npos) ?
            str : str.substr(start);
    }

    inline static std::string ltrim(const std::string& str, const std::string& chars = " ") {
        auto end = str.find_last_not_of(chars);
        return (end == std::string::npos) ?
            str : str.substr(0, end + 1);
    }

    inline static std::string trim(const std::string& str, const std::string& chars = " ") {
        return ltrim(rtrim(str, chars), chars);
    }
}