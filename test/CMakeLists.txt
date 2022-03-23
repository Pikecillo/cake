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

add_executable(test_disjoint_set
    test_disjoint_set.cpp
)
target_link_libraries(test_disjoint_set
    cake
    gtest
    gtest_main
    pthread
)

add_executable(test_hash
    test_hash.cpp
)
target_link_libraries(test_hash
    cake
    gtest
    gtest_main
    pthread
)

add_executable(test_lru_cache
    test_lru_cache.cpp
)
target_link_libraries(test_lru_cache
    cake
    gtest
    gtest_main
    pthread
)

add_executable(test_prefix_tree
    test_prefix_tree.cpp
)
target_link_libraries(test_prefix_tree
    cake
    gtest
    gtest_main
    pthread
)