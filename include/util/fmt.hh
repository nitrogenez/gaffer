#pragma once

#include <gaffer.hh>

#include <string>
#include <string_view>
#include <format>

namespace gaffer::util::colorio {
    enum class AnsiColorCode: u32 {
        Magenta = 35,
        Yellow = 33,
        Green = 32,
        Black = 30,
        White = 37,
        Blue = 34,
        Cyan = 36,
        Red = 31
    };

    enum class AnsiStyleCode: u32 {
        Reset = 0,
        Bold = 1,
        Underline = 4
    };

    inline static std::string color_to_string(AnsiColorCode code) {
        return "\033[" + std::to_string((u32)code) + "m";
    }

    inline static std::string style_to_string(AnsiStyleCode code) {
        return "\033[" + std::to_string((u32)code) + "m";
    }
}

namespace gaffer::util {
    struct Color {
        inline static const std::string
            Red = colorio::color_to_string(colorio::AnsiColorCode::Red),
            Green = colorio::color_to_string(colorio::AnsiColorCode::Green),
            Blue = colorio::color_to_string(colorio::AnsiColorCode::Blue),
            Yellow = colorio::color_to_string(colorio::AnsiColorCode::Yellow),
            Magenta = colorio::color_to_string(colorio::AnsiColorCode::Magenta),
            Cyan = colorio::color_to_string(colorio::AnsiColorCode::Cyan),
            Black = colorio::color_to_string(colorio::AnsiColorCode::Black),
            White = colorio::color_to_string(colorio::AnsiColorCode::White),

            Reset = colorio::style_to_string(colorio::AnsiStyleCode::Reset),
            Bold = colorio::style_to_string(colorio::AnsiStyleCode::Bold),
            Underline = colorio::style_to_string(colorio::AnsiStyleCode::Underline);
    };

    static inline std::string colored(
        std::string_view str,
        std::string_view color = "",
        std::string_view style = "")
    {
        return std::format("{}{}{}{}", color, style, str, Color::Reset);
    }
}