#include <gaffer.hh>
#include <util/string.hh>
#include <string>

namespace gaffer::util
{
    std::string rtrim(std::string const& str, std::string const& chars) {
        auto start = str.find_first_not_of(chars);
        return (start == std::string::npos) ?
            str : str.substr(start);
    }

    std::string ltrim(const std::string& str, const std::string& chars) {
        auto end = str.find_last_not_of(chars);
        return (end == std::string::npos) ?
            str : str.substr(0, end + 1);
    }

    std::string trim(std::string const& str, std::string const& chars) {
        return ltrim(rtrim(str, chars), chars);
    }
}