#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace petal {

enum class arg_type {
  positional,
  short_flag,
  long_flag,
};

struct arg {
  arg_type type;
  std::string_view content;
  std::optional<std::string_view> value{std::nullopt};
};

class args_parser {
public:
  args_parser(int argc, char **argv) {
    if (argc > 0) {
      _program_name = argv[0];
      _raw_args.reserve(static_cast<std::size_t>(argc) - 1);
    }

    for (int i = 1; i < argc; ++i)
      _raw_args.emplace_back(argv[i]);

    _parse_args();
  }

  const std::string &program_name() const { return _program_name; }
  const std::vector<std::string> &raw_args() const { return _raw_args; }

private:
  std::string _program_name;
  std::vector<std::string> _raw_args;
  std::vector<arg> _args;

  void _parse_args();
};

} // namespace petal
