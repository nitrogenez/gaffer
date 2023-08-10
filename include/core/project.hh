#pragma once

#include <gaffer.hh>
#include <core/person.hh>
#include <core/version.hh>
#include <string>
#include <sstream>
#include <vector>

namespace gaffer::core {

    enum class ProjectType {
        None,
        App,
        SharedLibrary,
        StaticLibrary
    };
    constexpr ProjectType type_from_str(std::string const& str);

    struct ProjectData {
        std::string name;
        std::string description;
        Version version;

        std::vector<std::string> license;
        std::vector<PersonData> authors;
    };

    enum class ProjectValidity {
        None,
        Valid,
        Invalid,
        SemiValid
    };

    class Project {
    public:
        ProjectData data;
        Project();
        virtual ~Project() {}

        ProjectValidity is_valid() const;

        static bool create_new(const std::string& name, ProjectType type);
    };
}

namespace std {
    string to_string(gaffer::core::ProjectType const& project_type);
    string to_string(gaffer::core::ProjectData const& data);
    string to_string(gaffer::core::ProjectValidity const& validity);
}