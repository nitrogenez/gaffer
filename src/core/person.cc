#include <gaffer.hh>
#include <core/person.hh>
#include <ostream>
#include <string>

namespace gaffer::core
{
    PersonData::operator std::string() const {
        std::string out = username;

        if (!email.empty())
            out += " <" + email + ">";
        return out;
    }

    PersonData::PersonData(std::string const& str) {
        if (str.empty())
            return;

        usize email_start_pos = str.find_first_of('<');
        usize email_end_pos = str.find_last_of('>');

        if (email_start_pos != std::string::npos && email_end_pos != std::string::npos) {
            username = str.substr(0, email_start_pos);
            email = str.substr(email_start_pos + 1,
                email_end_pos - email_start_pos - 1);

            username = util::trim(username);
            email = util::trim(email);
        } else {
            username = str;
        }
    }

    std::ostream& operator<<(std::ostream& os, PersonData const& data) {
        os << (std::string)data;
        return os;
    }
}