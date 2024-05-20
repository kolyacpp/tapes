#pragma once

#include "subcommand.h"

#include <string>

class gen_config_subcommand : public subcommand {
public:
  gen_config_subcommand(CLI::App& app);
  ~gen_config_subcommand() override = default;

protected:
  void run() override;

private:
  std::string _filepath;
};
