#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <petal/output/ansi.hpp>
#include <string_view>

namespace petal {

enum class color : std::uint8_t {
  black,
  red,
  green,
  yellow,
  blue,
  magenta,
  cyan,
  white,

  default_fg,
  default_bg
};

enum class color_modifier : std::uint8_t {
  none = 0,
  background = 1 << 0,
  bright = 1 << 1,
};

enum class text_modifier : std::uint8_t {
  none,
  bold,
  dim,
  italic,
  underline,
  blink,
  reverse,
  hidden,
  strikethrough,
  double_underline,

  no_bold_dim,
  no_italic,
  no_underline,
  no_blink,
  no_reverse,
  no_hidden,
  no_strikethrough
};

constexpr color_modifier operator|(color_modifier lhs,
                                   color_modifier rhs) noexcept {
  return static_cast<color_modifier>(static_cast<std::uint8_t>(lhs) |
                                     static_cast<std::uint8_t>(rhs));
}

inline constexpr bool has_flag(color_modifier value,
                               color_modifier flag) noexcept {
  return (static_cast<std::uint8_t>(value) & static_cast<std::uint8_t>(flag)) !=
         0;
}

namespace detail {

namespace c = ansi::colors;

inline constexpr std::array<std::array<std::string_view, 8>, 4> color_table{{
    {c::black, c::red, c::green, c::yellow, c::blue, c::magenta, c::cyan,
     c::white},
    {c::bright_black, c::bright_red, c::bright_green, c::bright_yellow,
     c::bright_blue, c::bright_magenta, c::bright_cyan, c::bright_white},
    {c::bg_black, c::bg_red, c::bg_green, c::bg_yellow, c::bg_blue,
     c::bg_magenta, c::bg_cyan, c::bg_white},
    {c::bg_bright_black, c::bg_bright_red, c::bg_bright_green,
     c::bg_bright_yellow, c::bg_bright_blue, c::bg_bright_magenta,
     c::bg_bright_cyan, c::bg_bright_white},
}};

} // namespace detail

inline const std::string_view
to_string(color c, color_modifier m = color_modifier::none) noexcept {
  if (c == color::default_fg)
    return ansi::colors::default_fg;

  if (c == color::default_bg)
    return ansi::colors::default_bg;

  const std::size_t row = (has_flag(m, color_modifier::background) ? 2 : 0) |
                          (has_flag(m, color_modifier::bright) ? 1 : 0);

  return detail::color_table[row][static_cast<std::size_t>(c)];
}

inline constexpr std::string_view to_string(text_modifier m) noexcept {
  namespace mod = ansi::modifiers;

  switch (m) {
  case text_modifier::none:
    return {};

  case text_modifier::bold:
    return mod::bold;
  case text_modifier::dim:
    return mod::dim;
  case text_modifier::italic:
    return mod::italic;
  case text_modifier::underline:
    return mod::underline;
  case text_modifier::blink:
    return mod::blink;
  case text_modifier::reverse:
    return mod::reverse;
  case text_modifier::hidden:
    return mod::hidden;
  case text_modifier::strikethrough:
    return mod::strikethrough;
  case text_modifier::double_underline:
    return mod::double_underline;
  case text_modifier::no_bold_dim:
    return mod::no_bold_dim;
  case text_modifier::no_italic:
    return mod::no_italic;
  case text_modifier::no_underline:
    return mod::no_underline;
  case text_modifier::no_blink:
    return mod::no_blink;
  case text_modifier::no_reverse:
    return mod::no_reverse;
  case text_modifier::no_hidden:
    return mod::no_hidden;
  case text_modifier::no_strikethrough:
    return mod::no_strikethrough;
  }

  return {};
}

} // namespace petal
