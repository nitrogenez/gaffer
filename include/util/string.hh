#pragma once

#include <gaffer.hh>
#include <string>


namespace gaffer::util {
    std::string rtrim(std::string const& str, std::string const& chars = " ");
    std::string ltrim(std::string const& str, std::string const& chars = " ");
    std::string trim(std::string const& str, std::string const& chars = " ");
}