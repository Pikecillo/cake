enable_testing()

include_directories(${CAKE_HOME}/include)

find_package(OpenSSL REQUIRED)

add_executable(test_bloom_filter
    test_bloom_filter.cpp
)
target_link_libraries(test_bloom_filter
    cake
    gtest
    gtest_main
    pthread
)

add_executable(test_trie
    test_trie.cpp
)
target_link_libraries(test_trie
    cake
    gtest
    gtest_main
    pthread
)