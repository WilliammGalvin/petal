#include <cstddef>
#include <petal/input/args_parser.hpp>

namespace petal {

void args_parser::_parse_args() {
  bool in_positional = true;
  std::size_t i = 0;
  while (i < _raw_args.size()) {
    const auto &raw_arg = _raw_args[i];
    if (in_positional && raw_arg.starts_with('-'))
      in_positional = false;

    if (in_positional) {
      _args.push_back(arg{.type = arg_type::positional, .content = raw_arg});
      continue;
    }

    if (raw_arg.starts_with('-')) {
      // if (raw_arg.starts_with("--"))
      //   _parse_long_flag(raw_arg);
    }
  }
}

} // namespace petal
