#include <cctype>
#include <cstdlib>
#include <gaffer.hh>
#include <core/version.hh>
#include <iostream>
#include <ostream>
#include <string>

namespace gaffer::core
{
    Version::Version(u32 major, u32 minor, u32 patch,
        std::string const& pre_id, std::string const& build_id)
        : m_major(major), m_minor(minor), m_patch(patch),
        m_pre_id(pre_id), m_build_id(build_id) {}

    Version::Version(std::string const& str) {
        std::string major_num;
        std::string minor_num;
        std::string patch_num;

        usize found_dots = 0;

        for (auto const& c: str) {
            if (c == '-') break;

            if (c == '.') found_dots++;

            if (std::isdigit(c)) {
                switch (found_dots) {
                    case 0:
                        major_num += c;
                        break;
                    case 1:
                        minor_num += c;
                        break;
                    case 2:
                        patch_num += c;
                        break;
                    default:
                        break;
                }
            }
        }
        m_major = std::atoi(major_num.c_str());
        m_minor = std::atoi(minor_num.c_str());
        m_patch = std::atoi(patch_num.c_str());

        usize first_dash = str.find_first_of('-');
        usize last_dash = str.find_last_of('-');

        if (first_dash != std::string::npos) {
            if (last_dash != std::string::npos) {
                if (first_dash == last_dash) {
                    m_pre_id = str.substr(first_dash + 1);
                } else {
                    m_pre_id = str.substr(first_dash + 1, last_dash - first_dash - 1);
                    m_build_id = str.substr(last_dash + 1);
                }
            } else {
                m_pre_id = str.substr(first_dash + 1);
            }
        }
    }

    std::ostream& operator<<(std::ostream& os, Version const& version) {
        os << version.m_major << "." << version.m_minor << "." << version.m_patch;

        if (!version.m_pre_id.empty()) os << "-" << version.m_pre_id;
        if (!version.m_build_id.empty()) os << "-" << version.m_build_id;

        return os;
    }

    bool Version::operator==(Version const& other) const {
        return (m_major == other.m_major) &&
            (m_minor == other.m_minor) &&
            (m_patch == other.m_patch) &&
            (m_pre_id == other.m_pre_id) &&
            (m_build_id == other.m_build_id);
    }

    bool Version::operator!=(Version const& other) const {
        return !(*this == other);
    }

    // TODO: Implement prerelease and build id comparison
    bool Version::operator<(Version const& other) const {
        bool less_mmp = (m_major < other.m_major) ||
                (m_minor < other.m_minor) ||
                (m_patch < other.m_patch);

        if (m_pre_id.empty() && m_build_id.empty()) {
            return less_mmp;
        } else if (m_build_id.empty()) {
            return less_mmp;
        } else {
            return less_mmp;
        }
    }

    bool Version::operator>(Version const& other) const {
        return !(*this < other);
    }

    bool Version::operator<=(Version const& other) const {
        return *this < other || *this == other;
    }

    bool Version::operator>=(Version const& other) const {
        return *this > other || *this == other;
    }
}