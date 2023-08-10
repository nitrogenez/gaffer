#pragma once

#include <gaffer.hh>
#include <string>
#include <sstream>
#include <filesystem>
#include <source_location>

namespace fs = std::filesystem;

#define LOG(x) gaffer::core::Log(x, true)

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
    std::string color_of(LogLevel const& level);

    class Log {
    private:
        std::ostringstream m_cout_buffer;
        std::ostringstream m_file_buffer;
        std::ostringstream m_msg_buffer;

        bool m_to_file = true;
        static fs::path m_file_path;
        static bool m_file_ready;

    public:
        Log(LogLevel const& level,
            bool const& to_file = true,
            std::source_location const& location = std::source_location::current());

        static bool init_file();

        template<typename T>
        Log& operator<<(T const& value) {
            m_msg_buffer << value;
            return *this;
        }

        virtual ~Log();
    };
}

namespace std
{
    string to_string(gaffer::core::LogLevel const& level);
}

inline fs::path gaffer::core::Log::m_file_path;
inline bool gaffer::core::Log::m_file_ready = false;