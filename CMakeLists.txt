cmake_minimum_required(VERSION 3.14)
project(cake)

set(CMAKE_CXX_STANDARD 17)

set(CAKE_HOME ${CMAKE_SOURCE_DIR})

add_subdirectory(src)
add_subdirectory(test)