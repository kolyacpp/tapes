#pragma once

#include <chrono>

namespace tapes::util {

class delay_guard {
public:
  using delay = std::chrono::milliseconds;
  using clock_t = std::chrono::system_clock;

  delay_guard(delay d) noexcept;
  ~delay_guard();

  delay_guard(const delay_guard&) = delete;
  delay_guard& operator=(const delay_guard&) = delete;

  delay_guard(delay_guard&& other) noexcept;
  delay_guard& operator=(delay_guard&& other) noexcept;

private:
  clock_t::time_point _wait_until;
};

} // namespace tapes::util