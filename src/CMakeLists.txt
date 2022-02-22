find_package(OpenSSL REQUIRED)

include_directories(${CAKE_HOME}/include)

add_library(cake
    Hash.cpp
)

target_link_libraries(cake
    PRIVATE OpenSSL::SSL
)