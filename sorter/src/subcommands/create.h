#pragma once

#include "subcommand.h"

#include <string>

class create_subcommand : public subcommand {
public:
  create_subcommand(CLI::App& app);
  ~create_subcommand() override = default;

protected:
  void run() override;

private:
  std::string _filepath;
};
