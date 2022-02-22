cmake_minimum_required(VERSION 3.14)
project(cake)

enable_testing()

include_directories(../include)

add_executable(
  test
  main.cpp
)
target_link_libraries(
  test
  gtest
  gtest_main
  pthread
)

include(GoogleTest)

gtest_discover_tests(test)