#pragma once

#include "subcommand.h"

#include <tapes/tapes.h>

#include <string>

class sort_subcommand : public subcommand {
public:
  sort_subcommand(CLI::App& app);
  ~sort_subcommand() override = default;

protected:
  void run() override;

private:
  std::string _input_path;
  std::string _output_path;

  uint64_t _buffer_size = 1 << 16;
  uint64_t _tape_count = 2;

  struct delays {
    static constexpr auto def = tapes::file_tape::default_delays;
    uint32_t read = def.read.count();
    uint32_t write = def.write.count();
    uint32_t move = def.move.count();
    uint32_t rewind = def.rewind.count();
    uint32_t clear = def.clear.count();
  } _delays;

  bool _order_asceding = true;
  bool _print_result = true;

  std::string _temp_diredtory = "system-default";
};
