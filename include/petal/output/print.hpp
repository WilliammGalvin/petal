#pragma once

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <petal/output/text.hpp>
#include <string_view>
#include <unistd.h>

namespace petal {

struct style {
  color fg{color::default_fg};
  color bg{color::default_bg};
  bool bright_bg{false};
  bool bright_fg{false};
  text_modifier tm{text_modifier::none};

  constexpr bool plain() const noexcept {
    return fg == color::default_fg && bg == color::default_bg &&
           tm == text_modifier::none;
  }
};

template <typename T> struct styled {
  const T &value;
  style st;
};

namespace detail {

template <typename> inline constexpr bool is_styled_v = false;
template <typename T> inline constexpr bool is_styled_v<styled<T>> = true;

inline bool colors_enabled_for(const std::ostream &os) {
  static const bool no_color = [] {
    const char *v = std::getenv("NO_COLOR");
    return v != nullptr && *v != '\0';
  }();

  static const bool out_tty = isatty(STDOUT_FILENO) != 0;
  static const bool err_tty = isatty(STDERR_FILENO) != 0;

  if (no_color)
    return false;

  if (&os == &std::cout)
    return out_tty;

  if (&os == &std::cerr || &os == &std::clog)
    return err_tty;

  return false;
}

template <typename T, typename F>
constexpr auto restyle(const T &v, F &&patch) {
  if constexpr (is_styled_v<T>) {
    T copy = v;
    patch(copy.st);
    return copy;
  } else {
    styled<T> s{v, {}};
    patch(s.st);
    return s;
  }
}

inline void write_style(std::ostream &os, const style &st) {
  using cm = color_modifier;
  if (st.fg != color::default_fg)
    os << to_string(st.fg, st.bright_fg ? cm::bright : cm::none);

  if (st.bg != color::default_bg)
    os << to_string(st.bg,
                    cm::background | (st.bright_bg ? cm::bright : cm::none));

  os << to_string(st.tm);
}

} // namespace detail

template <typename T>
constexpr auto fg(const T &v, color c, bool bright = false) {
  return detail::restyle(v, [=](style &s) {
    s.fg = c;
    s.bright_bg = bright;
  });
}

template <typename T>
constexpr auto bg(const T &v, color c, bool bright = false) {
  return detail::restyle(v, [=](style &s) {
    s.bg = c;
    s.bright_bg = bright;
  });
}

template <typename T> constexpr auto with_text(const T &v, text_modifier tm) {
  return detail::restyle(v, [=](style &s) { s.tm = tm; });
}

template <typename T> constexpr auto with_style(const T &v, style s) {
  return detail::restyle(v, [=](style &st) { st = s; });
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const styled<T> &s) {
  if (s.st.plain() || !detail::colors_enabled_for(os))
    return os << s.value;

  detail::write_style(os, s.st);
  os << s.value << ansi::reset;
  return os;
}

inline void print(std::string_view message, color c = color::default_fg,
                  color_modifier cm = color_modifier::none,
                  text_modifier tm = text_modifier::none) {
  const bool use_style = detail::colors_enabled_for(std::cout) &&
                         (c != color::default_fg || tm != text_modifier::none);

  if (!use_style) {
    std::cout << message;
    return;
  }

  std::cout << to_string(c, cm) << to_string(tm) << message << ansi::reset;
}

inline void println(std::string_view message, color c = color::default_fg,
                    color_modifier cm = color_modifier::none,
                    text_modifier tm = text_modifier::none) {
  print(message, c, cm, tm);
  std::cout << '\n';
}

} // namespace petal
