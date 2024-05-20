#pragma once

#include <CLI/CLI.hpp>

class subcommand {
public:
  subcommand(CLI::App* command);

  virtual ~subcommand() = default;

protected:
  virtual void run() = 0;

protected:
  CLI::App& app;
  CLI::App& command;
};