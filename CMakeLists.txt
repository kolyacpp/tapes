cmake_minimum_required(VERSION 3.21)

project(tapes CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "Build tests" ON)
option(BUILD_CLI "Build cli sorter application" ON)
option(USE_SANITIZERS "Enable to build with undefined, leak and address sanitizers" OFF)
option(ENABLE_SLOW_TEST "Enable slow tests" OFF)

add_library(compile_options INTERFACE)

if(MSVC)
  target_compile_options(compile_options INTERFACE /W4 /permissive-)
  if(TREAT_WARNINGS_AS_ERRORS)
    target_compile_options(compile_options INTERFACE /WX)
  endif()
else()
  target_compile_options(compile_options INTERFACE -Wall -pedantic -Wextra -Wno-sign-compare)
  target_compile_options(compile_options INTERFACE -Wold-style-cast -Wextra-semi -Woverloaded-virtual -Wzero-as-null-pointer-constant)
  if(TREAT_WARNINGS_AS_ERRORS)
    target_compile_options(compile_options INTERFACE -Werror -pedantic-errors)
  endif()
  target_compile_options(compile_options INTERFACE -Wno-self-move)
endif()

# Compiler specific warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(compile_options INTERFACE -Wshadow=compatible-local)
  target_compile_options(compile_options INTERFACE -Wduplicated-branches)
  target_compile_options(compile_options INTERFACE -Wduplicated-cond)
  target_compile_options(compile_options INTERFACE -Wno-array-bounds)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(compile_options INTERFACE -Wshadow-uncaptured-local)
  target_compile_options(compile_options INTERFACE -Wloop-analysis)
  target_compile_options(compile_options INTERFACE -Wno-self-assign-overloaded)
endif()

if(USE_SANITIZERS)
  message(STATUS "Enabling sanitizers...")
  target_compile_options(compile_options INTERFACE -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
  target_link_options(compile_options INTERFACE -fsanitize=address,undefined,leak)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(STATUS "Enabling libc++...")
  target_compile_options(compile_options INTERFACE -stdlib=libc++)
  target_link_options(compile_options INTERFACE -stdlib=libc++)
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  message(STATUS "Enabling _GLIBCXX_DEBUG...")
  target_compile_options(compile_options INTERFACE -D_GLIBCXX_DEBUG)
endif()

add_subdirectory(./tapes)

if (${BUILD_CLI})
  add_subdirectory(./third_party/CLI11)
  add_subdirectory(./sorter)
endif()

if (${ENABLE_TESTS})
  add_subdirectory(./third_party/googletest)
  add_subdirectory(./tests/tapes)
endif()
