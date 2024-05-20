#include "gen_config.h"

#include <tapes/tape.h>

#include <fstream>

gen_config_subcommand::gen_config_subcommand(CLI::App& app)
    : subcommand(app.add_subcommand("gen-config", "Generate a default configuration file")) {
  command.add_option("config-path", _filepath, "Path to the output config file")->configurable(false)->required();
}

void gen_config_subcommand::run() {
  std::ofstream file(CLI::to_path(_filepath));

  if (!file.is_open()) {
    throw CLI::Error("config", "Failed to open config file for writing: " + _filepath, 1);
  }

  file << app.config_to_str(true, false);
}
