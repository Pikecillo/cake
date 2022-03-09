#include <gtest/gtest.h>

#include <cake/BloomFilter.h>

TEST(BloomFilterTest, emptyFilter) {
    cake::BloomFilter bloomFilter(10000, 0.01);

    ASSERT_FALSE(bloomFilter.contains(9.3));
    ASSERT_FALSE(bloomFilter.contains(9));
}

TEST(BloomFilterTest, add) {
    cake::BloomFilter bloomFilter(200, 0.05);

    ASSERT_FALSE(bloomFilter.contains(7));
    bloomFilter.add(7);
    ASSERT_TRUE(bloomFilter.contains(7));
}

TEST(BloomFilterTest, clear) {
    cake::BloomFilter bloomFilter(10000, 0.05);

    bloomFilter.add(7);
    bloomFilter.add("eight");
    bloomFilter.add(9);
    bloomFilter.add(10);

    ASSERT_TRUE(bloomFilter.contains(7));
    ASSERT_TRUE(bloomFilter.contains("eight"));
    ASSERT_TRUE(bloomFilter.contains(9));
    ASSERT_TRUE(bloomFilter.contains(10));

    bloomFilter.clear();

    ASSERT_FALSE(bloomFilter.contains(7));
    ASSERT_FALSE(bloomFilter.contains("eight"));
    ASSERT_FALSE(bloomFilter.contains(9));
    ASSERT_FALSE(bloomFilter.contains(10));
}

TEST(BloomFilterTest, falsePositiveRate) {
    double tolerance = 1.1; // Deviation of 10% allowed
    double falsePositiveRate = 0.01;

    {
        const int numElements = 20000000;
        cake::BloomFilter bloomFilter(numElements, falsePositiveRate);
        int falsePositives = 0, totalAttempts = 0;

        for (int i = 0; i < numElements; i++) {
            if (!bloomFilter.contains(i)) {
                bloomFilter.add(i);
            } else {
                falsePositives++;
            }

            totalAttempts++;
        }

        EXPECT_LT(static_cast<double>(falsePositives) / totalAttempts,
                  tolerance * falsePositiveRate);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}