#include <tapes/util/delay_guard.h>

#include <algorithm>
#include <thread>
#include <utility>

namespace tapes::util {

using clock_t = delay_guard::clock_t;

delay_guard::delay_guard(delay d) noexcept : _wait_until(clock_t::now() + d) {}

delay_guard::delay_guard(delay_guard&& other) noexcept
    : _wait_until(std::exchange(other._wait_until, clock_t::time_point())) {}

delay_guard& delay_guard::operator=(delay_guard&& other) noexcept {
  if (this != &other) {
    _wait_until = std::exchange(other._wait_until, clock_t::time_point());
  }
  return *this;
}

delay_guard::~delay_guard() {
  // noexcept:
  // cppref: Any exception thrown by Clock or Duration
  // (clocks and durations provided by the standard library never throw).
  std::this_thread::sleep_until(_wait_until);
}

} // namespace tapes::util
