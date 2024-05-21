add_executable(tapes_tests_correctness correctness.cpp)

target_link_libraries(tapes_tests_correctness PRIVATE compile_options)
target_link_libraries(tapes_tests_correctness PRIVATE tapes GTest::gtest GTest::gtest_main)

add_executable(tapes_tests_random random.cpp)
target_link_libraries(tapes_tests_random PRIVATE tapes GTest::gtest GTest::gtest_main)

if (${ENABLE_SLOW_TEST})
  target_compile_definitions(tapes_tests_random PRIVATE ENABLE_SLOW_TEST)
endif()
