#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace petal::ansi {

inline constexpr std::string_view reset = "\033[0m";

namespace modifiers {
inline constexpr std::string_view bold = "\033[1m";
inline constexpr std::string_view dim = "\033[2m";
inline constexpr std::string_view italic = "\033[3m";
inline constexpr std::string_view underline = "\033[4m";
inline constexpr std::string_view blink = "\033[5m";
inline constexpr std::string_view reverse = "\033[7m";
inline constexpr std::string_view hidden = "\033[8m";
inline constexpr std::string_view strikethrough = "\033[9m";
inline constexpr std::string_view double_underline = "\033[21m";

inline constexpr std::string_view no_bold_dim = "\033[22m";
inline constexpr std::string_view no_italic = "\033[23m";
inline constexpr std::string_view no_underline = "\033[24m";
inline constexpr std::string_view no_blink = "\033[25m";
inline constexpr std::string_view no_reverse = "\033[27m";
inline constexpr std::string_view no_hidden = "\033[28m";
inline constexpr std::string_view no_strikethrough = "\033[29m";
} // namespace modifiers

namespace colors {
inline constexpr std::string_view black = "\033[30m";
inline constexpr std::string_view red = "\033[31m";
inline constexpr std::string_view green = "\033[32m";
inline constexpr std::string_view yellow = "\033[33m";
inline constexpr std::string_view blue = "\033[34m";
inline constexpr std::string_view magenta = "\033[35m";
inline constexpr std::string_view cyan = "\033[36m";
inline constexpr std::string_view white = "\033[37m";
inline constexpr std::string_view default_fg = "\033[39m";

inline constexpr std::string_view bright_black = "\033[90m";
inline constexpr std::string_view bright_red = "\033[91m";
inline constexpr std::string_view bright_green = "\033[92m";
inline constexpr std::string_view bright_yellow = "\033[93m";
inline constexpr std::string_view bright_blue = "\033[94m";
inline constexpr std::string_view bright_magenta = "\033[95m";
inline constexpr std::string_view bright_cyan = "\033[96m";
inline constexpr std::string_view bright_white = "\033[97m";

inline constexpr std::string_view bg_black = "\033[40m";
inline constexpr std::string_view bg_red = "\033[41m";
inline constexpr std::string_view bg_green = "\033[42m";
inline constexpr std::string_view bg_yellow = "\033[43m";
inline constexpr std::string_view bg_blue = "\033[44m";
inline constexpr std::string_view bg_magenta = "\033[45m";
inline constexpr std::string_view bg_cyan = "\033[46m";
inline constexpr std::string_view bg_white = "\033[47m";
inline constexpr std::string_view default_bg = "\033[49m";

inline constexpr std::string_view bg_bright_black = "\033[100m";
inline constexpr std::string_view bg_bright_red = "\033[101m";
inline constexpr std::string_view bg_bright_green = "\033[102m";
inline constexpr std::string_view bg_bright_yellow = "\033[103m";
inline constexpr std::string_view bg_bright_blue = "\033[104m";
inline constexpr std::string_view bg_bright_magenta = "\033[105m";
inline constexpr std::string_view bg_bright_cyan = "\033[106m";
inline constexpr std::string_view bg_bright_white = "\033[107m";
} // namespace colors

inline constexpr std::string_view clear_line = "\033[2K";
inline constexpr std::string_view clear_to_eol = "\033[K";
inline constexpr std::string_view clear_screen = "\033[2J";
inline constexpr std::string_view cursor_home = "\033[H";
inline constexpr std::string_view hide_cursor = "\033[?25l";
inline constexpr std::string_view show_cursor = "\033[?25h";

[[nodiscard]] inline std::string fg256(std::uint8_t n) {
  return "\033[38;5;" + std::to_string(n) + "m";
}

[[nodiscard]] inline std::string bg256(std::uint8_t n) {
  return "\033[48;5;" + std::to_string(n) + "m";
}

[[nodiscard]] inline std::string fg_rgb(std::uint8_t r, std::uint8_t g,
                                        std::uint8_t b) {
  return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" +
         std::to_string(b) + "m";
}

[[nodiscard]] inline std::string bg_rgb(std::uint8_t r, std::uint8_t g,
                                        std::uint8_t b) {
  return "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" +
         std::to_string(b) + "m";
}

} // namespace petal::ansi
