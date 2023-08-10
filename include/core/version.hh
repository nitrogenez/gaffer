#pragma once

#include <gaffer.hh>
#include <ostream>
#include <string>

namespace gaffer::core
{
    class Version {
    private:
        u32 m_major;
        u32 m_minor;
        u32 m_patch;

        std::string m_pre_id;
        std::string m_build_id;

    public:
        Version(u32 major = 0, u32 minor = 0, u32 patch = 0,
            std::string const& pre_id = "", std::string const& build_id = "");
        Version(std::string const& str);

        inline constexpr u32 get_major() const { return m_major; }
        inline constexpr u32 get_minor() const { return m_minor; }
        inline constexpr u32 get_patch() const { return m_patch; }

        inline constexpr std::string get_prerelease() const { return m_pre_id; }
        inline constexpr std::string get_build() const { return m_build_id; }

        friend std::ostream& operator<<(std::ostream& os, Version const& version);

        bool operator==(Version const& other) const;
        bool operator!=(Version const& other) const;
        bool operator<(Version const& other) const;
        bool operator>(Version const& other) const;
        bool operator<=(Version const& other) const;
        bool operator>=(Version const& other) const;
    };
}