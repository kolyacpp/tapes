#pragma once

#include "subcommand.h"

#include <tapes/tape.h>

#include <string>

class print_subcommand : public subcommand {
public:
  print_subcommand(CLI::App& app);
  ~print_subcommand() override = default;

  static void print_tape(tapes::itape& t, std::ostream&);

protected:
  void run() override;

private:
  std::string _filepath;
};
