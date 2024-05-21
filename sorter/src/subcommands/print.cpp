#include "print.h"

#include <tapes/tapes.h>

#include <cassert>

print_subcommand::print_subcommand(CLI::App& app)
    : subcommand(app.add_subcommand("print", "Prints values from tape file")) {
  command.add_option("filepath", _filepath, "Path to the output tape file")->required()->configurable(false);
}

void print_subcommand::print_tape(tapes::itape& t, std::ostream&) {
  size_t count = 0;
  while (auto opt = t.read()) {
    t.move_right();
    std::cout << *opt << ' ';
    if (++count % 10 == 0) {
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}

void print_subcommand::run() {
  using namespace tapes;
  file_tape t(CLI::to_path(_filepath), file_tape::mode::read);
  print_tape(t, std::cout);
}
