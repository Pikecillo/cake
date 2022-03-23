include_directories(${CAKE_HOME}/include)

add_library(cake SHARED
    BloomFilter.cpp
    DisjointSet.cpp
    Hash.cpp
    MurmurHash2.cpp
    LRUCache.cpp
    PrefixTree.cpp
)
