#pragma once

#include <gaffer.hh>
#include <util/string.hh>

#include <ostream>
#include <string>
#include <iostream>


namespace gaffer::core {

    struct PersonData {
        std::string username;
        std::string email;

        PersonData()
            : username("nil"), email("") {}

        PersonData(const std::string& str) {
            *this = from_str(str);
        }

        operator std::string() const {
            std::string out = username;

            if (!email.empty()) {
                out += " <" + email + ">";
            }
            return out;
        }

        inline static PersonData from_str(const std::string& str) {
            PersonData out;

            if (str.empty()) {
                return out;
            }

            usize
                email_start_pos = str.find_first_of('<'),
                email_end_pos = str.find_last_of('>');

            if (email_start_pos != std::string::npos && email_end_pos != std::string::npos) {
                out.username = str.substr(0, email_start_pos);
                out.email = str.substr(email_start_pos + 1, email_end_pos - email_start_pos - 1);

                out.username = util::str::trim(out.username);
                out.email = util::str::trim(out.email);
            } else {
                out.username = str;
            }

            return out;
        }
    };

    inline std::ostream& operator<<(std::ostream& os, const PersonData& pd) {
        os << (std::string)pd;
        return os;
    }
}
