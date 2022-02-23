find_package(OpenSSL REQUIRED)

include_directories(${CAKE_HOME}/include)

add_library(cake
    BloomFilter.cpp
    Hash.cpp
)

target_link_libraries(cake
    PRIVATE OpenSSL::SSL
)