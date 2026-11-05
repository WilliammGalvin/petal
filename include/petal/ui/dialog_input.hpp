#pragma once

#include <cstddef>
#include <petal/input/console_input.hpp>

namespace petal {

enum class wrap_mode { clamp, wrap };

template <std::size_t Count, wrap_mode Mode = wrap_mode::clamp>
class dialog_index {
  static_assert(Count > 0, "petal: dialog_index needs at least one item");

public:
  constexpr dialog_index() noexcept = default;

  constexpr explicit dialog_index(std::size_t index) noexcept
      : _index(index < Count ? index : Count - 1) {}

  constexpr void increment() noexcept {
    if (_index + 1 < Count)
      ++_index;
    else if constexpr (Mode == wrap_mode::wrap)
      _index = 0;
  }

  constexpr void decrement() noexcept {
    if (_index > 0)
      --_index;
    else if constexpr (Mode == wrap_mode::wrap)
      _index = Count - 1;
  }

  constexpr std::size_t index() const noexcept { return _index; }
  static constexpr std::size_t count() noexcept { return Count; }

  friend constexpr bool operator==(const dialog_index &a,
                                   const dialog_index &b) = default;

private:
  std::size_t _index{0};
};

enum class dialog_action { none, redraw, accept, cancel };

template <std::size_t Count, wrap_mode Mode>
constexpr dialog_action handle_key(dialog_index<Count, Mode> &index,
                                   key k) noexcept {
  const auto before = index;
  switch (k) {
  case key::enter:
    return dialog_action::accept;

  case key::escape:
  case key::eof:
    return dialog_action::cancel;

  case key::up:
  case key::left:
    index.decrement();
    break;

  case key::down:
  case key::right:
    index.increment();
    break;

  default:
    return dialog_action::none;
  }

  return index == before ? dialog_action::none : dialog_action::redraw;
}

} // namespace petal
