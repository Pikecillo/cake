#include <gtest/gtest.h>

#include <cake/Hash.h>

TEST(HashTest, alwaysSameResult) {
    {
        const int64_t value1 = 9;
        const int64_t value2 = 9;
        const auto hash1 = cake::Hash::murmur64A(value1, 0);
        const auto hash2 = cake::Hash::murmur64A(value2, 0);

        EXPECT_EQ(hash1, hash2);
    }
    {
        const char value1 = 'z';
        const char value2 = 'z';
        const auto hash1 = cake::Hash::murmur64A(value1, 1);
        const auto hash2 = cake::Hash::murmur64A(value2, 1);

        EXPECT_EQ(hash1, hash2);
    }
    {
        const std::string value1 = "PieceOfCake";
        const std::string value2 = "PieceOfCake";

        const auto hash1 = cake::Hash::murmur64A(value1, 2022);
        const auto hash2 = cake::Hash::murmur64A(value2, 2022);

        EXPECT_EQ(hash1, hash2);
    }
    {
        const std::vector<int> value1(50000, 77);
        const std::vector<int> value2(50000, 77);

        const auto hash1 = cake::Hash::murmur64A(value1, 2022);
        const auto hash2 = cake::Hash::murmur64A(value2, 2022);

        EXPECT_EQ(hash1, hash2);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}