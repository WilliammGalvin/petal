#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <petal/input/console_input.hpp>
#include <petal/output/ansi.hpp>
#include <petal/output/print.hpp>
#include <petal/ui/dialog_input.hpp>
#include <span>
#include <string_view>
#include <thread>

namespace petal {

struct default_label {
  template <typename U>
  constexpr std::string_view operator()(const U &value) const noexcept {
    return value;
  }
};

template <typename T, std::size_t N, typename ToString = default_label>
class multi_input {
  static_assert(N > 0, "petal: multi_input needs at least one choice");

public:
  constexpr multi_input(std::string_view message,
                        const std::array<T, N> &choices,
                        std::size_t starting_idx = 0, ToString to_str = {})
      : _message(message), _choices(choices), _to_str(std::move(to_str)),
        _index(starting_idx), _initial(_index.index()) {
    if (_choices.empty())
      throw std::invalid_argument("Choices cannot be empty");
  }

  [[nodiscard]] std::size_t get_choice() {
    raw_mode guard;
    _draw();

    key k{};
    while (true) {
      if (!read_key(k)) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(key_failed_fetch_delay_ms));
        continue;
      }

      switch (handle_key(_index, k)) {
      case dialog_action::redraw:
        _draw();
        break;

      case dialog_action::accept:
        std::cout << '\n';
        return _index.index();

      case dialog_action::cancel:
        std::cout << '\n';
        return _initial;

      case dialog_action::none:
        break;
      }
    }
  }

private:
  static constexpr std::size_t max_key_fetch_attempts = 3;
  static constexpr std::size_t key_failed_fetch_delay_ms = 10;

  std::string_view _message;
  std::span<const T, N> _choices;
  [[no_unique_address]] ToString _to_str;
  dialog_index<N> _index;
  std::size_t _initial;

  void _draw() const {
    std::cout << '\r' << ansi::clear_to_eol << _message << " [";
    for (std::size_t i = 0; i < N; ++i) {
      if (i > 0)
        std::cout << '/';

      const bool is_selected = i == _index.index();
      std::cout << (is_selected ? ansi::modifiers::bold : std::string_view{})
                << _to_str(_choices[i])
                << (is_selected ? ansi::reset : std::string_view{});
    }

    std::cout << "]" << std::flush;
  }
};

} // namespace petal
