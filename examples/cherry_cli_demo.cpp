#include <array>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <petal/output/print.hpp>
#include <petal/petal.hpp>
#include <petal/ui/bool_input.hpp>
#include <petal/ui/multi_input.hpp>
#include <string_view>

namespace {

using namespace petal;

int cmd_echo(args_view args) {
  std::cout << "Arguments (" << args.size() << "):\n";
  for (std::size_t i = 0; i < args.size(); ++i)
    std::cout << "  [" << i << "] " << args[i] << '\n';

  return 0;
}

int cmd_greet(args_view args) {
  if (args.empty()) {
    std::cerr << "usage: cherry greet <name>...\n";
    return exit_usage;
  }

  for (const std::string_view name : args)
    std::cout << "Hello, "
              << with_text(fg(name, color::magenta, /*bright=*/true),
                           text_modifier::bold)
              << "!\n";
  return 0;
}

struct swatch {
  std::string_view name;
  color c;
};

int cmd_colors(args_view) {
  static constexpr std::array<swatch, 8> swatches{{
      {"black", color::black},
      {"red", color::red},
      {"green", color::green},
      {"yellow", color::yellow},
      {"blue", color::blue},
      {"magenta", color::magenta},
      {"cyan", color::cyan},
      {"white", color::white},
  }};

  for (const auto &[name, c] : swatches)
    std::cout << std::left << std::setw(9) << name << fg("normal", c) << "  "
              << fg("bright", c, true) << "  " << bg("  bg  ", c) << ' '
              << bg("  bright bg  ", c, true) << '\n';

  std::cout << '\n'
            << with_text("bold", text_modifier::bold) << ' '
            << with_text("dim", text_modifier::dim) << ' '
            << with_text("italic", text_modifier::italic) << ' '
            << with_text("underline", text_modifier::underline) << ' '
            << with_text("reverse", text_modifier::reverse) << ' '
            << with_text("strikethrough", text_modifier::strikethrough) << '\n';
  return 0;
}

int cmd_init(args_view) {
  static constexpr std::array<std::string_view, 3> levels{"debug", "info",
                                                          "warn"};

  println("Project setup", color::cyan, color_modifier::none,
          text_modifier::bold);

  multi_input level_prompt{"Log level?", levels, /*starting_idx=*/1};
  const std::size_t level = level_prompt.get_choice();
  const bool git = get_bool_input("Initialise a git repository?", true);

  std::cout << "\nlog level : " << fg(levels[level], color::green)
            << "\ngit       : "
            << fg(git ? "yes" : "no", git ? color::green : color::yellow)
            << '\n';
  return 0;
}

struct demo_cli {
  static constexpr std::string_view name = "cherry";
  static constexpr std::array<command, 4> commands{{
      {.name = "init",
       .description = "Interactive project setup (prompts)",
       .handler = &cmd_init},

      {.name = "greet",
       .description = "Greet people (styled output)",
       .handler = &cmd_greet},

      {.name = "colors",
       .description = "Show the colour and text-style palette",
       .handler = &cmd_colors},

      {.name = "echo",
       .description = "Print the arguments a handler receives",
       .handler = &cmd_echo},
  }};
};

using app = cli<demo_cli>;

static_assert(app::command_count() == 4);
static_assert(app::find("init")->handler == &cmd_init);
static_assert(app::find("nope") == nullptr);

} // namespace

int main(int argc, char **argv) { return app::run(argc, argv); }
