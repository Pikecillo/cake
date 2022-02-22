enable_testing()

include_directories(${CAKE_HOME}/include)

find_package(OpenSSL REQUIRED)

add_executable(test
    main.cpp
)
target_link_libraries(test
    cake
    gtest
    gtest_main
    pthread
)

include(GoogleTest)

gtest_discover_tests(test)