#include <filesystem>
#include <fstream>
#include <gaffer.hh>
#include <core/logger.hh>
#include <util/time.hh>
#include <util/fmt.hh>
#include <string>
#include <iostream>

namespace gaffer::core
{
    std::string color_of(LogLevel const& level) {
        using L = LogLevel;
        using C = util::Color;

        switch (level) {
            case L::Debug: return C::Magenta;
            case L::Info: return C::Green;
            case L::Warning: return C::Yellow;
            case L::Error: return C::Red;
            case L::Status_OK: return C::Green;
            case L::Status_FAILED: return C::Red;
            default:
                break;
        }
        return "";
    }

    Log::Log(LogLevel const& level,
            bool const& to_file,
            std::source_location const& location)
    {
        using namespace util;

        std::string timestamp = util::Time::local();
        m_to_file = to_file && m_file_ready;

        m_cout_buffer << std::format("[{}] {}: {}:{}: ({}:{}): ",
            timestamp,
            colored(std::to_string(level), color_of(level)),
            location.file_name(),
            location.function_name(),
            location.line(),
            location.column());

        if (m_to_file) {
            m_file_buffer << std::format("[{}]\n", timestamp);
            m_file_buffer << std::format("file = \"{}\"\n", location.file_name());
            m_file_buffer << std::format("func = \"{}\"\n", location.function_name());
            m_file_buffer << std::format("line = {}\n", location.line());
            m_file_buffer << std::format("column = {}\n", location.column());
            m_file_buffer << std::format("level = \"{}\"\n", std::to_string(level));
        }
    }

    bool Log::init_file() {
        std::string timestamp = util::Time::local();
        Log::m_file_path = fs::current_path() / ".gaffer" / "logs" / 
            (timestamp + ".toml");

        std::ofstream ofs {Log::m_file_path, std::ios::out};
        if (!ofs.is_open())
            return false;

        ofs << "# Initialized log file at " << timestamp << "\n";
        ofs.close();

        Log::m_file_ready = true;
        return true;
    }

    Log::~Log() {
        if (m_to_file) {
            m_file_buffer << std::format("msg = \"{}\"", m_msg_buffer.str());
            std::ofstream ofs {m_file_path};
            ofs << m_file_buffer.str() << std::endl;
            ofs.close();
        }
        m_cout_buffer << m_msg_buffer.str();
        std::cout << m_cout_buffer.str() << std::endl;

        m_file_buffer.clear();
        m_cout_buffer.clear();
        m_msg_buffer.clear();
    }
}

namespace std
{
    string to_string(gaffer::core::LogLevel const& level) {
        using L = gaffer::core::LogLevel;

        switch (level) {
            case L::Trace: return "Trace";
            case L::Debug: return "Debug";
            case L::Info: return "Info";
            case L::Warning: return "Warning";
            case L::Error: return "Error";
            case L::Status_OK: return "OK";
            case L::Status_FAILED: return "FAILED";
            default:
                break;
        }
        return "None";
    }
}