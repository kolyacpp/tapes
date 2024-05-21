#pragma once

#include <filesystem>

namespace tapes {

class tape_exception : public std::runtime_error {
public:
  using base = std::runtime_error;
  explicit tape_exception(const std::string& message);
};

} // namespace tapes
