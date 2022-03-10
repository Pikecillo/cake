#include <gtest/gtest.h>

#include <cake/BloomFilter.h>

TEST(BloomFilterTest, testConstructor) {
    {
        cake::BloomFilter bloomFilter(0, 0.00049);
        EXPECT_EQ(1, bloomFilter.expectedCount());
        EXPECT_EQ(0.0005, bloomFilter.falsePositiveRate());
    }
    {
        cake::BloomFilter bloomFilter(10, 0.00051);
        EXPECT_EQ(10, bloomFilter.expectedCount());
        EXPECT_EQ(0.00051, bloomFilter.falsePositiveRate());
    }
    {
        cake::BloomFilter bloomFilter(10, 0.500001);
        EXPECT_EQ(10, bloomFilter.expectedCount());
        EXPECT_EQ(0.5, bloomFilter.falsePositiveRate());
    }
    {
        cake::BloomFilter bloomFilter(10, 0.499999);
        EXPECT_EQ(10, bloomFilter.expectedCount());
        EXPECT_EQ(0.499999, bloomFilter.falsePositiveRate());
    }
}

TEST(BloomFilterTest, emptyFilter) {
    cake::BloomFilter bloomFilter(10000, 0.01);

    EXPECT_FALSE(bloomFilter.contains(9.3));
    EXPECT_FALSE(bloomFilter.contains(9));
}

TEST(BloomFilterTest, add) {
    cake::BloomFilter bloomFilter(200, 0.05);

    EXPECT_FALSE(bloomFilter.contains(7));
    bloomFilter.add(7);
    EXPECT_TRUE(bloomFilter.contains(7));
}

TEST(BloomFilterTest, clear) {
    cake::BloomFilter bloomFilter(10000, 0.05);

    bloomFilter.add(7);
    bloomFilter.add("eight");
    bloomFilter.add(9);
    bloomFilter.add(10);

    EXPECT_TRUE(bloomFilter.contains(7));
    EXPECT_TRUE(bloomFilter.contains("eight"));
    EXPECT_TRUE(bloomFilter.contains(9));
    EXPECT_TRUE(bloomFilter.contains(10));

    bloomFilter.clear();

    EXPECT_FALSE(bloomFilter.contains(7));
    EXPECT_FALSE(bloomFilter.contains("eight"));
    EXPECT_FALSE(bloomFilter.contains(9));
    EXPECT_FALSE(bloomFilter.contains(10));
}

TEST(BloomFilterTest, falsePositiveRate) {
    double tolerance = 1.1; // Deviation of 10% allowed
    double falsePositiveRate = 0.01;

    {
        const int numElements = 10000000;
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