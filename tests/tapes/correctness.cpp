#include "util.h"

#include <gtest/gtest.h>
#include <tapes/tapes.h>
#include <tapes/util/delay_guard.h>

#include <filesystem>

using namespace tapes;

namespace fs = std::filesystem;

TEST(correctness, temp_file) {
  fs::path path;
  {
    temp_file file("prefix");
    path = file.get_path();
    ASSERT_TRUE(std::filesystem::exists(path));
  }

  ASSERT_FALSE(std::filesystem::exists(path));
}

TEST(correctness, file_tape_create_empty) {
  auto [file, tape] = TEMP_TAPE();
  ASSERT_FALSE(tape.read());
}

TEST(correctness, file_tape_movable) {
  auto [file, tape] = TEMP_TAPE();

  write_tape(tape, {1, 2, 3});

  file_tape moved = std::move(tape);

  expect_tape(moved, {1, 2, 3});
}

void test_read_write(itape& tape) {
  auto min = std::numeric_limits<itape::int_type>::min();
  auto max = std::numeric_limits<itape::int_type>::max();

  itape::int_type arr[] = {max, min, 0, 1, 100, 50, 70, 20};

  for (auto value : arr) {
    tape.write(value);
    tape.move_right();
  }

  tape.rewind();

  for (auto value : arr) {
    auto read_value = tape.read();
    ASSERT_TRUE(read_value);
    ASSERT_EQ(*read_value, value);
    tape.move_right();
  }

  ASSERT_FALSE(tape.read());
}

TEST(correctness, file_tape_read_write) {
  auto [file, tape] = TEMP_TAPE();
  test_read_write(tape);
}

TEST(correctness, file_tape_move_left) {
  auto [file, tape] = TEMP_TAPE();
  write_tape(tape, {1, 2, 3});

  tape.move_left();
  ASSERT_EQ(*tape.read(), 3);

  tape.move_left();
  ASSERT_EQ(*tape.read(), 2);

  tape.move_left();
  ASSERT_EQ(*tape.read(), 1);
}

TEST(correctness, vector_tape_create_empty) {
  vector_tape tape;
  ASSERT_FALSE(tape.read());
}

TEST(correctness, vector_tape_copyable) {
  vector_tape tape({1, 2, 3});
  vector_tape copy = tape;

  expect_tape(tape, {1, 2, 3});
  expect_tape(copy, {1, 2, 3});
}

TEST(correctness, vector_tape_read_write) {
  vector_tape tape;
  test_read_write(tape);
}

TEST(correctness, sort) {
  for (Comp comp : {std::less<itape::int_type>(), std::less<itape::int_type>()}) {
    std::vector<itape::int_type> values = {100, 5, 20, 30, 25, 1, 2};

    vector_tape input(values);
    vector_tape output;

    tapes::multiway_merge_sort(3, 3, comp).sort(input, output);

    std::ranges::sort(values, comp);
    ASSERT_EQ(output.get_vector(), values);
  }
}

TEST(correctness, delay_guard) {
  using namespace std::chrono_literals;
  auto now = std::chrono::system_clock::now;

  auto time = now();
  {
    tapes::util::delay_guard guard(100ms);
    ASSERT_LE(now() - time, 2ms);
  }
  ASSERT_GE(now() - time, 100ms);
  ASSERT_LE(now() - time, 102ms);
}

TEST(correctness, file_tape_delay) {
  using namespace std::chrono_literals;
  auto now = std::chrono::system_clock::now;

  temp_file file("delay_move");

  file_tape::delays delays = {
      .write = 50ms,
      .read = 60ms,
      .move = 70ms,
      .rewind = 80ms,
      .clear = 90ms,
  };

  file_tape tape(file.get_path(), file_tape::mode::write | file_tape::mode::read, delays);

#define TAPES_ASSERT_DELAY(action, delay)                                                                              \
  {                                                                                                                    \
    auto time = now();                                                                                                 \
    action;                                                                                                            \
    auto eslaped = now() - time;                                                                                       \
    ASSERT_GE(eslaped, delay);                                                                                         \
    ASSERT_LE(eslaped - 2ms, delay);                                                                                   \
  }

  TAPES_ASSERT_DELAY(tape.write(1), delays.write);
  TAPES_ASSERT_DELAY(tape.read(), delays.read);
  TAPES_ASSERT_DELAY(tape.move_right(), delays.move);
  TAPES_ASSERT_DELAY(tape.move_left(), delays.move);
  TAPES_ASSERT_DELAY(tape.rewind(), delays.rewind);
  TAPES_ASSERT_DELAY(tape.clear(), delays.clear);
}
