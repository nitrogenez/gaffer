#pragma once

#include <gaffer.hh>
#include <util/string.hh>
#include <string>

namespace gaffer::core {

    struct PersonData {
        std::string username;
        std::string email;

        PersonData(): username("nil"), email("") {}

        PersonData(std::string const& str);
        operator std::string() const;
    };
    std::ostream& operator<<(std::ostream& os, PersonData const& pd);
}
