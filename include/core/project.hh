#pragma once

#define TOML_EXCEPTIONS 0

#include <gaffer.hh>
#include <core/person.hh>

#include <format>
#include <fstream>
#include <ios>
#include <vector>
#include <array>
#include <string>
#include <filesystem>
#include <toml.hh>

namespace gaffer::core {
    namespace fs = std::filesystem;

    constexpr const std::array<std::string, 10> illegal_project_names {
        "lib",
        "project",
        "package",
        "pkg",
        "test",
        "docs",
        "doc",
        "static_lib",
        "shared_lib",
        "library"
    };

    enum class ProjectType {
        None = 0,
        App,
        SharedLibrary,
        StaticLibrary
    };

    struct ProjectData {
        std::string name;
        std::string description;

        std::vector<std::string> licenses;
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

    class Project {
    public:
        ProjectData data;

        Project() {
            fs::path cwd = fs::current_path();
            fs::path proj_path = cwd / "project.toml";

            if (!fs::exists(proj_path)) {
                std::cerr << "No project found in the current directory" << std::endl;
                return;
            }

            toml::parse_result result = toml::parse_file(proj_path.string());

            if (!result) {
                std::cerr << "Failed to parse project.toml: " << result.error() << std::endl;
                return;
            }
            toml::table proj_info = result.table();

            data.name = proj_info["project"]["name"].value_or<std::string>("");
            data.description = proj_info["project"]["description"].value_or<std::string>("");

            if (toml::array* arr = proj_info["project"]["authors"].as_array()) {
                arr->for_each([this](auto&& e) {
                    if constexpr (toml::is_string<decltype(e)>) {
                        data.authors.emplace_back(*e);
                    }
                });
            }

            if (toml::array* arr = proj_info["project"]["license"].as_array()) {
                arr->for_each([this](auto&& e) {
                    if constexpr (toml::is_string<decltype(e)>) {
                        data.licenses.emplace_back(*e);
                    }
                });
            }
        }

        inline static bool is_valid(const Project& proj) {
            return
                !proj.data.name.empty();
        }

        inline static bool create_new(const std::string& name, ProjectType type) {
            for (const auto &n : illegal_project_names) {
                if (n == name) {
                    std::cerr << "    Project name '" << name << "' is reserved by Gaffer." << std::endl;
                    return false;
                }
            }

            // Initialize paths
            fs::path cwd = fs::current_path();
            fs::path p_root = cwd / name;

            fs::path p_file = p_root / "project.toml";
            fs::path p_src = p_root / "src";
            fs::path p_inc = p_root / "include";

            fs::path g_dir = p_root / ".gaffer";
            fs::path g_cache = g_dir / "cache";
            fs::path g_logs = g_dir / "logs";

            // Create all needed directories
            fs::create_directories(p_root);
            fs::create_directories(p_src);
            fs::create_directories(p_inc);
            fs::create_directories(g_dir);
            fs::create_directories(g_cache);
            fs::create_directories(g_logs);

            // Basic ofstream to open all needed project files
            std::ofstream file;

            file.open(p_file, std::ios::out);
            file
                << "[project]\n"
                << std::format("name = \"{}\"\n", name)
                << "version = \"0.1.0\"\n"
                << std::format("type = \"{}\"\n", ProjectTypeImpl::as_str(type))
                << "std = \"20\"\n\n"
                << "# For more info see Gaffer docs\n\n"
                << "[opts]\n\n[deps]" << std::endl;
            file.close();

            file.open(p_inc / std::format("{}.hh", name));
            file
                << "#pragma once\n\n"
                << "#include <cstddef>\n"
                << "#include <cstdint>\n\n"
                << std::format("namespace {}", name) << "{\n"
                << "    typedef std::uint64_t u64;\n"
                << "    typedef std::uint32_t u32;\n"
                << "    typedef std::uint16_t u16;\n"
                << "    typedef std::uint8_t u8;\n\n"
                << "    typedef std::int64_t i64;\n"
                << "    typedef std::int32_t i32;\n"
                << "    typedef std::int16_t i16;\n"
                << "    typedef std::int8_t i8;\n\n"
                << "    typedef std::size_t usize;\n"
                << "    typedef std::intmax_t ssize;\n"
                << "}" << std::endl;
            file.close();

            if (type == ProjectType::App) {
                file.open(p_src / "main.cc", std::ios::out);
                file
                    << std::format("#include <{}.hh>\n", name)
                    << "#include <iostream>\n\n"
                    << std::format("using namespace {};\n\n", name)
                    << "i32 main(i32 argc, char** argv) {\n"
                    << "    std::cout << \"Hello, World!\" << std::endl;\n"
                    << "}" << std::endl;
                file.close();
            }

            else {
                file.open(p_src / "lib.cc", std::ios::out);
                file << std::format("#include <{}.hh>", name) << std::endl;
                file.close();
            }

            fs::current_path(p_root);
            if (!Project::is_valid({})) {
                return false;
            }
            return true;
        }

        operator bool() const {
            return Project::is_valid(*this);
        }
    };
}