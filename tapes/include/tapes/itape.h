#pragma once

#include <cstdint>
#include <optional>

namespace tapes {

class itape {
public:
  using int_type = uint32_t;
  virtual ~itape() = default;
  virtual void write(uint32_t) = 0;
  virtual std::optional<uint32_t> read() = 0;
  virtual void move_left() = 0;
  virtual void move_right() = 0;
  virtual void rewind() = 0;
  virtual void clear() = 0;
};

} // namespace tapes
