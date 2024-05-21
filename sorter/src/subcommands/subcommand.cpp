#include "subcommand.h"

#include <CLI/CLI.hpp>

subcommand::subcommand(CLI::App* command) : app(*command->get_parent()), command(*command) {
  command->callback([this]() { run(); });
}
