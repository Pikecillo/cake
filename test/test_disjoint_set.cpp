/**
 * MIT License
 * 
 * Copyright (c) 2022 Mario Rincon Nigro
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <gtest/gtest.h>

#include <cake/DisjointSet.h>

TEST(DisjointSetTest, add) {
    cake::DisjointSet<int> disjointSet;
    const auto id1 = disjointSet.add(1);
    const auto id2 = disjointSet.add(2);

    EXPECT_NE(id1, id2);

    const auto id3 = disjointSet.add(2);
    EXPECT_EQ(id2, id3);
}

TEST(DisjointSetTest, find) {
    cake::DisjointSet<int> disjointSet;
    const auto id1 = disjointSet.add(1);
    const auto id2 = disjointSet.add(2);
    const auto id3 = disjointSet.add(3);

    const auto actual1 = disjointSet.find(1);
    const auto actual2 = disjointSet.find(2);
    const auto actual3 = disjointSet.find(3);

    EXPECT_EQ(id1, actual1.value());
    EXPECT_EQ(id2, actual2.value());
    EXPECT_EQ(id3, actual3.value());

    const auto actual4 = disjointSet.find(4);
    EXPECT_FALSE(actual4.has_value());
}

TEST(DisjointSetTest, join) {
    cake::DisjointSet<int> disjointSet;
    const auto id1 = disjointSet.add(1);
    const auto id2 = disjointSet.add(2);
    const auto id3 = disjointSet.add(3);
    const auto id4 = disjointSet.add(4);

    EXPECT_TRUE(disjointSet.join(1, 5));
    EXPECT_FALSE(disjointSet.join(1, 5));
    EXPECT_TRUE(disjointSet.join(-1, 4));

    EXPECT_TRUE(disjointSet.join(id1, id2));
    EXPECT_FALSE(disjointSet.join(id1, id2));

    EXPECT_TRUE(disjointSet.join(3, 4));
    EXPECT_FALSE(disjointSet.join(3, 4));
    EXPECT_FALSE(disjointSet.join(id3, id4));

    const auto actualId1 = disjointSet.find(-1);
    const auto actualId2 = disjointSet.find(4);

    ASSERT_TRUE(actualId1.has_value());
    ASSERT_TRUE(actualId2.has_value());
    EXPECT_EQ(actualId1.value(), actualId2.value());

    EXPECT_TRUE(disjointSet.join(id2, id3));
    EXPECT_FALSE(disjointSet.join(id2, id4));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}