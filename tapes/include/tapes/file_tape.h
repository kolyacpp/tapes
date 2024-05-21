#pragma once

#include "itape.h"

#include <filesystem>
#include <fstream>

namespace tapes {

class file_tape final : public itape {
public:
  using int_type = itape::int_type;

  struct mode {
    static constexpr uint8_t read = 0b01;
    static constexpr uint8_t write = 0b10;
  };

  using delay = std::chrono::milliseconds;

  struct delays {
    delay write;
    delay read;
    delay move;
    delay rewind;
    delay clear;
  };

  static constexpr delays default_delays = {};

  file_tape(const std::filesystem::path& filepath, uint8_t open_mode = mode::read | mode::write,
            delays d = default_delays);
  ~file_tape() override = default;

  file_tape(const file_tape&) = delete;
  file_tape& operator=(const file_tape&) = delete;

  file_tape(file_tape&&) = default;
  file_tape& operator=(file_tape&&) = default;

  void write(int_type value) override;
  std::optional<int_type> read() override;
  void move_left() noexcept override;
  void move_right() noexcept override;
  void rewind() noexcept override;
  void clear() override;

  void close_file();

private:
  delays _delays;
  std::filesystem::path _filepath;
  std::fstream _file;
  std::streampos _position;
};

} // namespace tapes
