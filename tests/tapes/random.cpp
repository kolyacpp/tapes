#include "util.h"

#include <gtest/gtest.h>
#include <tapes/tapes.h>

#include <random>

using namespace tapes;

using test_arguments = std::tuple<uint64_t, size_t, size_t, Comp>;

class random_sort : public testing::TestWithParam<test_arguments> {};

TEST_P(random_sort, is_sorted) {
  auto [iterations, buffer_size, tape_count, comp] = GetParam();
  auto [file, tape] = TEMP_TAPE();

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<itape::int_type> dist(std::numeric_limits<itape::int_type>::min(),
                                                      std::numeric_limits<itape::int_type>::max());

  for (uint64_t it = 0; it < iterations; ++it) {
    tape.write(dist(gen));
    tape.move_right();
  }

  tape.rewind();

  auto [file2, tape2] = TEMP_TAPE();

  tapes::multiway_merge_sort(buffer_size, tape_count, comp).sort(tape, tape2);

  uint64_t count = 0;
  tape2.rewind();
  auto next_value = tape2.read();
  while (next_value) {
    ++count;
    tape2.move_right();
    auto value = *next_value;
    if ((next_value = tape2.read())) {
      ASSERT_TRUE(!comp(*next_value, value));
    }
  }

  ASSERT_EQ(count, iterations);
}

INSTANTIATE_TEST_SUITE_P(random_sort_tests, random_sort,
                         testing::Values(
                             /* iterations, buffer_size, tape_count, comparator */
                             test_arguments{100, 2, 2, std::less<>()}, test_arguments{100, 2, 2, std::greater<>()},
                             test_arguments{10000, 5, 5, std::less<>()}, test_arguments{10000, 5, 5, std::greater<>()}

#ifdef ENABLE_SLOW_TEST
                             ,
                             test_arguments{5000000, 100000, 5, std::less<>()},
                             test_arguments{5000000, 100000, 5, std::greater<>()}
#endif
                             ));

class counting_sort : public testing::TestWithParam<test_arguments> {};

TEST_P(counting_sort, counting_check) {
  auto [iterations, buffer_size, tape_count, comp] = GetParam();
  auto [file, tape] = TEMP_TAPE();

  constexpr itape::int_type min = 0;
  constexpr itape::int_type max = 100000;

  std::vector<itape::int_type> count(max - min + 1, 0);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<itape::int_type> dist(min, max);

  for (uint64_t it = 0; it < iterations; ++it) {
    auto val = dist(gen);
    count[val - min]++;
    tape.write(val);
    tape.move_right();
  }

  tape.rewind();

  auto [file2, tape2] = TEMP_TAPE();

  tapes::multiway_merge_sort(buffer_size, tape_count, comp).sort(tape, tape2);

  tape2.rewind();
  for (auto i = min; i <= max; i++) {
    while (count[i - min] > 0) {
      --count[i - min];
      auto val = tape2.read();
      tape2.move_right();
      ASSERT_TRUE(val);
      ASSERT_EQ(*val, i);
    }
  }
  ASSERT_FALSE(tape2.read());
}

INSTANTIATE_TEST_SUITE_P(random_sort_counting_test, counting_sort,
                         testing::Values(
                             /* iterations, buffer_size, tape_count, comparator */
                             test_arguments{10, 2, 2, std::less<>()}, test_arguments{10000, 5, 5, std::less<>()}

#ifdef ENABLE_SLOW_TEST
                             ,
                             test_arguments{5000000, 100000, 5, std::less<>()}
#endif
                             ));
