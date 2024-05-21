#pragma once

#include <gtest/gtest.h>
#include <tapes/tapes.h>

#include <utility>

namespace {

using Comp = std::function<bool(tapes::itape::int_type, tapes::itape::int_type)>;

std::pair<tapes::temp_file, tapes::file_tape> create_temp_tape(size_t prefix) {
  tapes::temp_file temp("test_correctness" + std::to_string(prefix));
  return std::make_pair(std::move(temp), tapes::file_tape(temp.get_path()));
}

#define TEMP_TAPE() create_temp_tape(__LINE__)

template <typename Container = std::initializer_list<tapes::itape::int_type>>
void write_tape(tapes::itape& to, const Container& list) {
  for (auto value : list) {
    to.write(value);
    to.move_right();
  }
}

template <typename Container = std::initializer_list<tapes::itape::int_type>>
void expect_tape(tapes::itape& tape, const Container& list) {
  tape.rewind();
  for (auto value : list) {
    auto read_value = tape.read();
    ASSERT_TRUE(read_value);
    ASSERT_EQ(*read_value, value);
    tape.move_right();
  }
}

} // namespace
