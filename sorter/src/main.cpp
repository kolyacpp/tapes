#include "subcommands/create.h"
#include "subcommands/gen_config.h"
#include "subcommands/print.h"
#include "subcommands/sort.h"

#include <CLI/CLI.hpp>
#include <tapes/tapes.h>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  CLI::App app{"Tapes cli"};
  app.require_subcommand(1);
  argv = app.ensure_utf8(argv);

  create_subcommand create(app);
  print_subcommand print(app);
  sort_subcommand sort(app);
  gen_config_subcommand gen_config(app);

  CLI11_PARSE(app, argc, argv);

  return 0;
}
