#pragma once

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace petal {

using args_view = std::span<const std::string_view>;
using handler_fn = int (*)(args_view);

inline constexpr int exit_ok = 0;
inline constexpr int exit_usage = 2;

struct command {
  std::string_view name;
  std::string_view description;
  handler_fn handler{nullptr};
};

template <typename T>
concept cli_config = requires {
  { T::name } -> std::convertible_to<std::string_view>;
  { T::commands } -> std::ranges::input_range;
  requires std::same_as<std::ranges::range_value_t<decltype(T::commands)>,
                        command>;
};

struct empty_cli_config {
  static constexpr std::string_view name = "";
  static constexpr std::array<command, 0> commands{};
};

namespace detail {

inline constexpr std::string_view reserved_help = "help";

template <cli_config C>
constexpr std::size_t command_count_v = std::size(C::commands);

template <cli_config C> consteval bool names_valid() {
  for (const command &cmd : C::commands) {
    if (cmd.name.empty() || cmd.name.front() == '-')
      return false;

    for (const char ch : cmd.name) {
      if (ch == ' ' || ch == '\t' || ch == '\n')
        return false;
    }

    return true;
  }
}

template <cli_config C> consteval bool handlers_set() {
  for (const command &cmd : C::commands) {
    if (cmd.handler == nullptr)
      return false;
  }

  return true;
}

template <cli_config C> consteval bool names_unique() {
  const auto &cmds = C::commands;
  for (std::size_t i = 0; i < std::size(cmds); ++i) {
    for (std::size_t j = i + 1; j < std::size(cmds); ++j) {
      if (cmds[i].name == cmds[j].name)
        return false;
    }

    return true;
  }
}

template <cli_config C> consteval bool no_reserved_names() {
  for (const command &cmd : C::commands) {
    if (cmd.name == reserved_help)
      return false;
  }

  return true;
}

template <cli_config C> consteval auto make_command_table() {
  std::array<command, command_count_v<C>> table{};
  std::ranges::copy(C::commands, table.begin());
  std::ranges::sort(table, {}, &command::name);
  return table;
}

template <cli_config C> consteval std::size_t longest_name() {
  std::size_t longest = reserved_help.size();
  for (const command &cmd : C::commands)
    longest = std::max(longest, cmd.name.size());

  return longest;
}

} // namespace detail

template <cli_config Config = empty_cli_config> class cli {
  static_assert(detail::names_valid<Config>(),
                "petal: command names must be non-empty, contain no "
                "whitespace, and must not start with '-'");
  static_assert(detail::handlers_set<Config>(),
                "petal: every command must have a handler function");
  static_assert(detail::names_unique<Config>(),
                "petal: command names must be unique");
  static_assert(detail::no_reserved_names<Config>(),
                "petal: command names must not be reserved words");

public:
  static constexpr int exit_ok = petal::exit_ok;
  static constexpr int exit_usage = petal::exit_usage;

  static constexpr std::size_t command_count() noexcept {
    return detail::command_count_v<Config>;
  }

  static constexpr const auto &commands() noexcept { return Config::commands; }

  static constexpr const command *find(std::string_view name) noexcept {
    const auto it = std::ranges::lower_bound(_table, name, {}, &command::name);
    return it != _table.end() && it->name == name ? &*it : nullptr;
  }

  [[nodiscard]] static int run(int argc, const char *const *argv,
                               std::ostream &out = std::cout,
                               std::ostream &err = std::cerr) {
    std::vector<std::string_view> args;
    if (argc > 1)
      args.assign(argv + 1, argv + argc);

    return dispatch(args, out, err);
  }

  [[nodiscard]] static int dispatch(args_view args,
                                    std::ostream &out = std::cout,
                                    std::ostream &err = std::cerr) {
    if (args.empty()) {
      print_help(err);
      return exit_usage;
    }

    const std::string_view first = args.front();
    if (first == detail::reserved_help || first == "-h" || first == "--help") {
      print_help(out);
      return exit_ok;
    }

    if (const command *cmd = find(first))
      return cmd->handler(args.subspan(1));

    err << _program_name() << ": unknown command '" << first << "'\n"
        << "Run '" << _program_name() << " help' for usage.\n";
    return exit_usage;
  }

  static void print_help(std::ostream &os) {
    constexpr std::size_t width = detail::longest_name<Config>();
    os << "Usage: " << _program_name() << " <command> [args...]\n\n";
    os << "Commands:\n";
    for (const command &cmd : _table)
      _row(os, cmd.name, cmd.description, width);
    _row(os, detail::reserved_help, "Show this help message", width);
  }

private:
  static constexpr auto _table = detail::make_command_table<Config>();

  static constexpr std::string_view _program_name() noexcept {
    return Config::name.empty() ? std::string_view{"app"} : Config::name;
  }

  static void _row(std::ostream &os, std::string_view name,
                   std::string_view description, std::size_t width) {
    os << " ";
    if (description.empty()) {
      os << name << "\n";
      return;
    }

    os << std::left << std::setw(static_cast<int>(width)) << name << "  "
       << description << "\n";
  }
};

} // namespace petal
