#pragma once

#include <gaffer.hh>
#include <util/fmt.hh>
#include <util/time.hh>

#include <iostream>
#include <ios>
#include <string>
#include <format>
#include <sstream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

#define LOG(x) gaffer::core::Log(x, true, __FILE_NAME__, __LINE__)

#define log_trace LOG(gaffer::core::LogLevel::Trace)
#define log_debug LOG(gaffer::core::LogLevel::Debug)
#define log_info LOG(gaffer::core::LogLevel::Info)
#define log_warn LOG(gaffer::core::LogLevel::Warning)
#define log_err LOG(gaffer::core::LogLevel::Error)
#define status_ok LOG(gaffer::core::LogLevel::Status_OK)
#define status_err LOG(gaffer::core::LogLevel::Status_FAILED)

namespace gaffer::core
{
    enum class LogLevel {
        None = 0,
        Trace,
        Debug,
        Info,
        Warning,
        Error,

        Status_OK,
        Status_FAILED
    };

    struct LogLevelImpl {
        static constexpr std::string as_str(LogLevel ll) {
            using l = LogLevel;
            switch (ll) {
                case l::Trace: return "trace";
                case l::Debug: return "debug";
                case l::Info: return "info";
                case l::Warning: return "warning";
                case l::Error: return "error";
                case l::Status_OK: return "ok";
                case l::Status_FAILED: return "failed";
                default: break;
            }
            return "";
        }

        static constexpr LogLevel from_str(const std::string& s) {
            using l = LogLevel;
            if (s == "trace") return l::Trace;
            else if (s == "debug") return l::Debug;
            else if (s == "info") return l::Info;
            else if (s == "warning") return l::Warning;
            else if (s == "error") return l::Error;
            else if (s == "ok") return l::Status_OK;
            else if (s == "failed") return l::Status_FAILED;
            else return l::None;
        }

        static constexpr std::string color_of(LogLevel ll) {
            using l = LogLevel;
            switch (ll) {
                case l::Debug: return util::Color::Magenta + util::Color::Bold;
                case l::Info: return util::Color::Green + util::Color::Bold;
                case l::Warning: return util::Color::Yellow + util::Color::Bold;
                case l::Error: return util::Color::Red + util::Color::Bold + util::Color::Underline;
                case l::Status_OK: return util::Color::Green + util::Color::Bold;
                case l::Status_FAILED: return util::Color::Red + util::Color::Bold;
                default: break;
            }
            return "";
        }
    };

    class Log {
    private:
        std::ostringstream m_cout_buffer;
        std::ostringstream m_file_buffer;
        std::ostringstream m_msg_buffer;

        bool m_to_file = true;
        static fs::path m_file_path;
        static bool m_file_ready;

    public:
        Log(
            LogLevel level,
            bool to_file = true,
            std::string scope = "unknown_scope",
            i32 line = 0)
        {
            std::string time = util::Time::local();
            m_to_file = to_file && m_file_ready;

            m_cout_buffer << std::format("[{}] {}: {}: ",
                time,
                util::colored(LogLevelImpl::as_str(level), LogLevelImpl::color_of(level)),
                scope);

            if (to_file) {
                // TOML log output
                m_file_buffer << std::format("[{}]\n", time);
                m_file_buffer << std::format("scope = \"{}\"\n", scope);
                m_file_buffer << std::format("level = \"{}\"", LogLevelImpl::as_str(level));
            }
        }

        static bool init_file() {
            std::string timestamp = util::Time::local();
            Log::m_file_path = fs::current_path() / ".gaffer" / "logs" / (timestamp + ".toml");

            std::ofstream ofs {Log::m_file_path, std::ios::out};

            if (!ofs.is_open()) {
                return false;
            }

            ofs << "# Launch log at " << timestamp << "\n";
            ofs.close();

            Log::m_file_ready = true;
        }

        template<typename T>
        Log& operator<<(const T& value) {
            m_msg_buffer << value;
        }

        virtual ~Log() {
            if (m_to_file) {
                m_file_buffer << std::format("msg = \"{}\"\n", m_msg_buffer.str());

                std::ofstream ofs {m_file_path};
                ofs << m_file_buffer.str() << std::endl;
                ofs.close();
            }
            m_cout_buffer << m_msg_buffer.str();
            std::cout << m_cout_buffer.str() << std::endl;
        }
    };
}

inline fs::path gaffer::core::Log::m_file_path;
inline bool gaffer::core::Log::m_file_ready = false;