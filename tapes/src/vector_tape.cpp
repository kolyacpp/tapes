#include <tapes/vector_tape.h>

#include <cassert>

namespace tapes {

vector_tape::vector_tape(std::initializer_list<int_type> list) : _vector(std::move(list)) {}

void vector_tape::write(int_type value) {
  if (_position >= _vector.size()) {
    _vector.push_back(value);
  } else {
    _vector[_position] = value;
  }
}

std::optional<vector_tape::int_type> vector_tape::read() noexcept {
  if (_position >= _vector.size()) {
    return std::nullopt;
  }
  return _vector[_position];
}

void vector_tape::move_left() noexcept {
  assert(_position > 0);
  --_position;
}

void vector_tape::move_right() noexcept {
  ++_position;
}

void vector_tape::rewind() noexcept {
  _position = 0;
}

void vector_tape::clear() noexcept {
  _vector.clear();
  rewind();
}

const std::vector<vector_tape::int_type>& vector_tape::get_vector() const noexcept {
  return _vector;
}

} // namespace tapes
