cmake_minimum_required(VERSION 3.14)
project(cake)

find_package(OpenSSL REQUIRED)

include_directories(${CAKE_HOME}/include)

target_link_libraries(
  OpenSSL::SSL
)

add_library(cake STATIC
    Hash.cpp
)