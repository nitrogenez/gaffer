#include <cstdlib>
#include <filesystem>
#include <gaffer.hh>
#include <core/project.hh>
#include <stdexcept>
#include <util/fmt.hh>
#include <argparse.hh>

#include <iostream>

using namespace argparse;
using namespace gaffer;

int main(int argc, char** argv) {
    ArgumentParser cli {"gaffer", GAFFER_VERSION};

    cli.add_argument("-C")
        .help("Change directory before doing anything")
        .default_value("")
        .action([](auto& path) {
            if (std::filesystem::exists(path)) {
                if (std::filesystem::is_directory(path)) {
                    std::filesystem::current_path(path);
                } else {
                    std::cerr << "'" << path << "' is not a directory!" << std::endl;
                    std::exit(1);
                }
            } else {
                std::cerr << "'" << path << "': No such file or directory" << std::endl;
                std::exit(1);
            }
        })
        .metavar("PATH");

    ArgumentParser cli_validate {"validate"};
    ArgumentParser cli_info {"info"};
    ArgumentParser cli_new {"new"};

    cli_validate.add_description("Validate the current project");
    cli_info.add_description("Show the current project info");
    cli_new.add_description("Create a new project");

    cli_new.add_argument("name")
        .help("Name of the project to create");

    // New command flags
    cli_new.add_argument("--app")
        .help("Create an app project")
        .default_value(false)
        .implicit_value(true);
    cli_new.add_argument("--lib")
        .help("Create a static library project")
        .default_value(false)
        .implicit_value(true);
    cli_new.add_argument("--static")
        .help("Create a static library project")
        .default_value(false)
        .implicit_value(true);
    cli_new.add_argument("--shared")
        .help("Create a shared library project")
        .default_value(false)
        .implicit_value(true);

    cli.add_subparser(cli_validate);
    cli.add_subparser(cli_info);
    cli.add_subparser(cli_new);

    try {
        cli.parse_args(argc, argv);
    } catch (const std::runtime_error &e) {
        std::cerr << "  " << e.what() << std::endl;
        return 0;
    }

    if (cli.is_subcommand_used("validate")) {
        gaffer::core::Project project;

        if (!std::filesystem::exists("project.toml")) {
            return 1;
        }

        if (!project) {
            std::cout << "  The project is "
                << util::colored("invalid", util::Color::Red, util::Color::Bold)
                << std::endl;
            return 1;
        } else {
            std::cout << "  The project is "
                << util::colored("valid", util::Color::Green, util::Color::Bold)
                << std::endl;
            return 0;
        }
    }

    else if (cli.is_subcommand_used("new")) {
        if (std::filesystem::exists(cli_new.get("name"))) {
            std::cerr << "    Project '" << cli_new.get("name") << "' already exists" << std::endl;
            return 0;
        }

        core::ProjectType t;

        if (cli_new.is_used("--lib") || cli_new.is_used("--static"))
            t = core::ProjectType::StaticLibrary;
        else if (cli_new.is_used("--shared"))
            t = core::ProjectType::SharedLibrary;
        else
            t = core::ProjectType::App;

        bool result = core::Project::create_new(cli_new.get("name"), t);

        if (!result) {
            std::cerr << "  Something went wrong when creating a new project" << std::endl;
            return 1;
        }
        std::cout
            << "     "
            << util::colored("Created", util::Color::Green, util::Color::Bold)
            << " "
            << core::ProjectTypeImpl::as_str_pretty(t)
            << " project '" << cli_new.get("name") << "'" << std::endl;
    }

    return 0;
}