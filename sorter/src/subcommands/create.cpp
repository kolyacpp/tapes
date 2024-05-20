#include "create.h"

#include <tapes/tapes.h>

#include <cassert>

create_subcommand::create_subcommand(CLI::App& app)
    : subcommand(app.add_subcommand("create", "Creates a new tape file with values")) {
  command.add_option("filepath", _filepath, "Path to the output tape file")->required()->configurable(false);
}

void create_subcommand::run() {
  tapes::file_tape tape(CLI::to_path(_filepath));
  tape.clear();

  int64_t value;
  while(std::cin >> value) {
    if(value < 0) {
      break;
    }
    tape.write(value);
    tape.move_right();
  }
}
