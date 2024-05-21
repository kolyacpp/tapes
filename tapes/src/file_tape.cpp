#include <tapes/file_tape.h>
#include <tapes/tape_exception.h>
#include <tapes/util/delay_guard.h>

#include <cassert>

namespace tapes {

using util::delay_guard;

file_tape::file_tape(const std::filesystem::path& filepath, uint8_t open_mode, delays d)
    : _delays(d),
      _filepath(filepath),
      _position(0) {
  auto modes = std::ios::binary;
  if (open_mode & mode::read) {
    modes |= std::ios::in;
  }

  if (open_mode & mode::write) {
    modes |= std::ios::out;
  }

  _file.open(filepath, modes);

  if (_file.is_open()) {
    return;
  }

  if (open_mode & mode::write) {
    std::ofstream(filepath).close();
    _file.open(filepath, modes);

    if (_file.is_open()) {
      return;
    }
  }

  throw tape_exception("Failed to open file: " + filepath.string());
}

void file_tape::write(int_type value) {
  delay_guard guard(_delays.write);
  _file.seekp(_position, std::ios_base::beg);
  _file.write(reinterpret_cast<const char*>(&value), sizeof(value));
  if (_file.fail()) {
    throw tape_exception("Failed to write to file: " + _filepath.string());
  }
}

std::optional<file_tape::int_type> file_tape::read() {
  delay_guard guard(_delays.read);
  int_type value;
  _file.seekg(_position, std::ios_base::beg);
  _file.read(reinterpret_cast<char*>(&value), sizeof(value));
  if (_file.eof()) {
    _file.clear();
    return std::nullopt;
  }
  if (_file.fail()) {
    throw tape_exception("Failed to read from file: " + _filepath.string());
  }

  return value;
}

void file_tape::move_left() noexcept {
  delay_guard guard(_delays.move);
  assert(_position >= sizeof(int_type));
  _position -= sizeof(int_type);
}

void file_tape::move_right() noexcept {
  delay_guard guard(_delays.move);
  _position += sizeof(int_type);
}

void file_tape::rewind() noexcept {
  delay_guard guard(_delays.rewind);
  _position = 0;
}

void file_tape::clear() {
  delay_guard guard(_delays.clear);
  _position = 0;
  std::error_code ec;
  std::filesystem::resize_file(_filepath, 0, ec);
  if (ec) {
    throw tape_exception("Failed to clear file: " + _filepath.string());
  }
}

void file_tape::close_file() {
  _file.close();
}

} // namespace tapes
