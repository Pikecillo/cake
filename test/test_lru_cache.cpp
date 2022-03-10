#include <gtest/gtest.h>

#include <cake/LRUCache.h>

TEST(LRUCacheTest, test_insert) {
    cake::LRUCache<int, std::string> cache(2);

    EXPECT_EQ(cache.size(), 0);
    cache.insert(1, "Uno");
    EXPECT_EQ(cache.size(), 1);
    cache.insert(1, "One");
    EXPECT_EQ(cache.size(), 1);
    cache.insert(2, "Two");
    EXPECT_EQ(cache.size(), 2);
    cache.insert(3, "Three");
    EXPECT_EQ(cache.size(), 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}