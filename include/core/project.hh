#pragma once

#include <gaffer.hh>
#include <core/person.hh>
#include <string>
#include <sstream>
#include <vector>

namespace gaffer::core {

    enum class ProjectType {
        None = 0,
        App,
        SharedLibrary,
        StaticLibrary
    };

    struct ProjectData {
        std::string name;
        std::string description;

        std::vector<std::string> license;
        std::vector<PersonData> authors;
    };


    struct ProjectTypeImpl {
        static std::string as_str(ProjectType t) {
            switch (t) {
                case ProjectType::App: return "app";
                case ProjectType::SharedLibrary: return "shared_lib";
                case ProjectType::StaticLibrary: return "static_lib";
                default: break;
            }
            return "";
        }

        static std::string as_str_pretty(ProjectType t) {
            switch (t) {
                case ProjectType::App: return "application";
                case ProjectType::SharedLibrary: return "shared library";
                case ProjectType::StaticLibrary: return "static library";
                default: break;
            }
            return "";
        }

        static ProjectType from_str(const std::string& s) {
            if (s == "app") return ProjectType::App;
            else if (s == "shared_lib") return ProjectType::SharedLibrary;
            else if (s == "static_lib") return ProjectType::StaticLibrary;
            else return ProjectType::None;
        }
    };

    enum class ProjectValidity {
        None = 0,
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
    inline string to_string(gaffer::core::ProjectType t) {
        using T = gaffer::core::ProjectType;

        switch (t) {
            case T::App:
                return "app";
            case T::SharedLibrary:
                return "shared_lib";
            case T::StaticLibrary:
                return "static_lib";
            default:
                break;            
        }
        return "";
    }

    inline string to_string(gaffer::core::ProjectData d) {
        ostringstream os;

        os << "Project {";
        os << "name: " << d.name << ", ";
        os << "version: none";
        os << "description: " << d.description << ", ";
        os << "license: [";

        for (const auto& data : d.license) {
            os << data;

            if (d.license.back() != data) {
                os << ", ";
            }
        }

        os << "], ";
        os << "authors: [";

        for (const auto& data : d.authors) {
            os << data << ", ";
        }
        os << "]";
        return os.str();
    }

    inline string to_string(gaffer::core::ProjectValidity v) {
        using V = gaffer::core::ProjectValidity;
        switch (v) {
            case V::Valid:
                return "Valid";
            case V::Invalid:
                return "Invalid";
            case V::SemiValid:
                return "SemiValid";
            default:
                break;
        }
        return "";
    }
}