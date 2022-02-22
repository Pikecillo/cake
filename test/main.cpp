#include <gtest/gtest.h>

#include <cake/BloomFilter.h>

TEST(BloomFilterTest, emptyFilter) {
    cake::BloomFilter<int> bloomFilter; 

    ASSERT_FALSE(bloomFilter.contains(7));
    ASSERT_FALSE(bloomFilter.contains(9));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}