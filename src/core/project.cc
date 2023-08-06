#define TOML_EXCEPTIONS 0
#define TOML_HEADER_ONLY 1

#include <gaffer.hh>
#include <core/project.hh>
#include <core/logger.hh>
#include <core/person.hh>
#include <filesystem>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <array>
#include <toml.hh>


namespace gaffer::core
{
    namespace fs = std::filesystem;

    constexpr const std::array<std::string, 12> names {
        "static_lib",
        "shared_lib",
        "dependency",
        "library",
        "project",
        "package",
        "tests",
        "test",
        "docs",
        "lib",
        "pkg",
        "doc"
    };

    Project::Project() {
        fs::path cwd = fs::current_path();
        fs::path project_path = cwd / "project.toml";

        // TODO: Replace std::cerr with the error log
        if (!fs::exists(project_path)) {
            log_err << "There is no project to be found";
            return;
        }

        toml::table project = {};
        if (toml::parse_result res = toml::parse_file(project_path.string())) {
            project = res.table();
        } else {
            log_err << "Unable to parse project: " << res.error();
            return;
        }

        this->data.name = project
            .at_path("project.name").value_or("");
        this->data.description = project
            .at_path("project.description").value_or("");

        if (toml::array* arr = project.at_path("project.authors").as_array()) {
            arr->for_each([this](auto&& e) {
                if constexpr (toml::is_string<decltype(e)>) {
                    this->data.authors.emplace_back(*e);
                }
            });
        }

        if (toml::array* arr = project.at_path("project.license").as_array()) {
            arr->for_each([this](auto&& e) {
                if constexpr (toml::is_string<decltype(e)>) {
                    this->data.license.emplace_back(*e);
                }
            });
        }
    }

    bool Project::create_new(const std::string& name, ProjectType type) {
        if (std::find(names.begin(), names.end(), name) != names.end()) {
            std::cerr << "The name '" << name << "' is reserved by Gaffer" << std::endl;
            return false;
        }

        // Paths
        fs::path cwd = fs::current_path();
        fs::path project_root = cwd / name;
        fs::path project_file = project_root / "project.toml";
        fs::path project_src = project_root / "src";
        fs::path project_inc = project_root / "include";
        fs::path gaffer_root = project_root / ".gaffer";
        fs::path gaffer_cache = gaffer_root / "cache";
        fs::path gaffer_logs = gaffer_root / "logs";

        // Create directories
        fs::create_directories(project_root);
        fs::create_directories(project_src);
        fs::create_directories(project_inc);
        fs::create_directories(gaffer_root);
        fs::create_directories(gaffer_cache);
        fs::create_directories(gaffer_logs);

        // Basic ofstream to open all needed project files
        std::ofstream file;

        // TODO: I think it'd be better, if we made this so
        // the user can change the template. But that's for
        // the future.
        file.open(project_file, std::ios::out);
        file
            << "[project]\n"
            << std::format("name = \"{}\"\n", name)
            << "version = \"0.1.0\"\n"
            << std::format("type = \"{}\"\n", ProjectTypeImpl::as_str(type))
            << "std = \"20\"\n\n"
            << "# For more info see Gaffer docs\n\n"
            << "[opts]\n\n[deps]" << std::endl;
        file.close();

        file.open(project_inc / std::format("{}.hh", name));
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
            file.open(project_src / "main.cc", std::ios::out);
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
            file.open(project_src / "lib.cc", std::ios::out);
            file << std::format("#include <{}.hh>", name) << std::endl;
            file.close();
        }

        // -Just a temporary workaround-
        // 'Kay, this is now a fucking monument
        // of my misery. I know how to make it work
        // better, but it's too risky, especially
        // in UNIX environment. I don't want to put anyone
        // at risk of an attack.
        fs::current_path(project_root);
        std::system("git init > /dev/null");
        fs::current_path(cwd);

        return true;
    }

    ProjectValidity Project::is_valid() const {
        using Validity = ProjectValidity;
        i32 points = 0;

        bool valid_name {
            !this->data.name.empty() &&
            !(std::find(names.begin(), names.end(),
                this->data.name) != names.end())
        };
        bool has_authors = !this->data.authors.empty();
        bool has_license = !this->data.license.empty();
        bool has_description = !this->data.description.empty();

        if (valid_name)
            points++;
        else points = -100;

        if (has_description) points++;
        if (has_license) points++;
        if (has_authors) points++;

        if (points <= 0)
            return Validity::Invalid;
        else if (points >= 0 && points < 4)
            return Validity::SemiValid;
        return Validity::Valid;
    }
}
