#include "sort.h"

#include "print.h"

#include <CLI/Error.hpp>
#include <tapes/tapes.h>

#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

sort_subcommand::sort_subcommand(CLI::App& app)
    : subcommand(app.add_subcommand("sort", "Sorts values from input tape file to output tape file")) {
  app.set_config("-c,--config", "config.conf", "Configuration file", false);

  command.add_option("input", _input_path, "Path to the input tape file")->required()->configurable(false);
  command.add_option("output", _output_path, "Path to the output tape file")->required()->configurable(false);

  command.add_option("-b,--buffer-size", _buffer_size, "Buffer size in bytes")
      ->configurable(true)
      ->capture_default_str();
  command.add_option("-t,--tape-count", _tape_count, "Tape size in bytes")->configurable(true)->capture_default_str();

  command.add_option("-o,--order-asc", _order_asceding, "Order asceding")->configurable(true)->capture_default_str();
  command.add_option("-p,--print-result", _print_result, "Print the result to stdout")
      ->configurable(true)
      ->capture_default_str();

  command.add_option("-r,--read-delay", _delays.read, "Read delay in milliseconds")
      ->configurable(true)
      ->capture_default_str();
  command.add_option("-w,--write-delay", _delays.write, "Write delay in milliseconds")
      ->configurable(true)
      ->capture_default_str();
  command.add_option("-m,--move-delay", _delays.move, "Move delay in milliseconds")
      ->configurable(true)
      ->capture_default_str();
  command.add_option("-R,--rewind-delay", _delays.rewind, "Rewind delay in milliseconds")
      ->configurable(true)
      ->capture_default_str();
  command.add_option("-c,--clear-delay", _delays.clear, "Clear delay in milliseconds")
      ->configurable(true)
      ->capture_default_str();
  command.add_option("-d,--temp-directory", _temp_diredtory, "Temp directory")
      ->configurable(true)
      ->capture_default_str();
}

void sort_subcommand::run() {
  using namespace tapes;

  if (_temp_diredtory != "system-default") {
    fs::create_directories(_temp_diredtory);
    tapes::temp_file::set_temp_directory(_temp_diredtory);
  }

  file_tape::delays delays = {.write = std::chrono::milliseconds(_delays.write),
                              .read = std::chrono::milliseconds(_delays.read),
                              .move = std::chrono::milliseconds(_delays.move),
                              .rewind = std::chrono::milliseconds(_delays.rewind),
                              .clear = std::chrono::milliseconds(_delays.clear)};

  file_tape input(_input_path, file_tape::mode::read, delays);
  file_tape output(_output_path, file_tape::mode::read | file_tape::mode::write, delays);

  if (_order_asceding) {
    tapes::multiway_merge_sort(_buffer_size, _tape_count, std::less<>()).sort(input, output);
  } else {
    tapes::multiway_merge_sort(_buffer_size, _tape_count, std::greater<>()).sort(input, output);
  }

  if (_print_result) {
    output.rewind();
    print_subcommand::print_tape(output, std::cout);
  }
}
