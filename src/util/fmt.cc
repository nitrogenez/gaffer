#include <gaffer.hh>
#include <util/fmt.hh>
#include <string>
#include <format>

namespace gaffer::util
{
    namespace colorio
    {
        std::string color_to_string(AnsiColorCode const& code) {
            return "\033[" + std::to_string((u32)code) + "m";
        }
        std::string style_to_string(AnsiStyleCode const& code) {
            return "\033[" + std::to_string((u32)code) + "m";
        }
    }

    std::string colored(std::string const& str,
        std::string const& color,
        std::string const& style)
    {
        return std::format("{}{}{}{}", color, style, str, Color::Reset);
    }
}