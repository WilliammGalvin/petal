#pragma once

#include <array>
#include <cstddef>
#include <petal/ui/multi_input.hpp>
#include <string_view>

namespace petal {

[[nodiscard]] inline bool get_bool_input(std::string_view message,
                                         bool default_value = false) {
  static constexpr std::array<std::string_view, 2> options{"yes", "no"};
  multi_input prompt{message, options, default_value ? 0uz : 1uz};
  return prompt.get_choice() == 0;
}

} // namespace petal
