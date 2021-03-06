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

#include <cake/LRUCache.h>

TEST(LRUCacheTest, test_constructor) {
    {
    cake::LRUCache<int, std::string> cache(0);

    EXPECT_EQ(1, cache.maxSize());
    EXPECT_EQ(0, cache.size());
    }

       {
    cake::LRUCache<int, std::string> cache(100);

    EXPECT_EQ(100, cache.maxSize());
    EXPECT_EQ(0, cache.size());
    }
}

TEST(LRUCacheTest, test_size_one_cache) {
    cake::LRUCache<int, std::string> cache(1);

    cache.insert(1, "One");
    EXPECT_TRUE(cache.contains(1));
    EXPECT_EQ(1, cache.size());

    cache[2] = "Two";
    EXPECT_TRUE(cache.contains(2));
    EXPECT_EQ(1, cache.size());

    cache.remove(2);
    EXPECT_EQ(0, cache.size());

    cache.insert(3, "Three");
    EXPECT_TRUE(cache.contains(3));
    EXPECT_EQ(1, cache.size());

    cache.insert(3, "Drei");
    EXPECT_TRUE(cache.contains(3));
    EXPECT_EQ(1, cache.size());
}

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

TEST(LRUCacheTest, test_remove) {
    cake::LRUCache<int, std::string> cache(2);

    cache.insert(1, "One");
    EXPECT_EQ(cache.size(), 1);
    
    cache.insert(2, "Two");
    EXPECT_EQ(cache.size(), 2);
    
    EXPECT_TRUE(cache.remove(1));
    EXPECT_EQ(cache.size(), 1);
    EXPECT_FALSE(cache.contains(1));
    EXPECT_TRUE(cache.contains(2));
    
    EXPECT_TRUE(cache.remove(2));
    EXPECT_EQ(cache.size(), 0);
    EXPECT_FALSE(cache.contains(2));

    EXPECT_FALSE(cache.remove(3));
    EXPECT_EQ(cache.size(), 0);
}

TEST(LRUCacheTest, test_access) {
    cake::LRUCache<int, std::string> cache(2);

    cache.insert(1, "One");
    EXPECT_EQ("One", cache[1]);

    cache[2];
    EXPECT_TRUE(cache.contains(2));

    cache[2] = "Two";
    EXPECT_EQ("Two", cache[2]);

    cache[3] = "Three";
    EXPECT_TRUE(cache.contains(2));
}

TEST(LRUCacheTest, test_contains) {
    cake::LRUCache<int, std::string> cache(2);

    EXPECT_FALSE(cache.contains(1));
    
    cache.insert(1, "Uno");
    EXPECT_TRUE(cache.contains(1));
    
    cache.insert(1, "One");
    EXPECT_TRUE(cache.contains(1));
    
    cache.insert(2, "Two");
    EXPECT_TRUE(cache.contains(2));
    
    cache.insert(3, "Three");
    EXPECT_TRUE(cache.contains(3));
    EXPECT_FALSE(cache.contains(1));
    EXPECT_TRUE(cache.contains(2));
}

TEST(LRUCacheTest, test_lru_eviction_policy) {
    cake::LRUCache<int, std::string> cache(4);

    cache.insert(1, "One");
    cache.insert(2, "Two");
    cache.insert(3, "Three");
    cache.insert(4, "Four"); // 4 3 2 1
    cache.insert(5, "Five"); // 5 4 3 2
    EXPECT_EQ(4, cache.size());
    EXPECT_FALSE(cache.contains(1));

    EXPECT_TRUE(cache.contains(2)); // 2 5 4 3
    cache.insert(6, "Six"); // 6 2 5 4
    EXPECT_EQ(4, cache.size());
    EXPECT_FALSE(cache.contains(3));

    EXPECT_EQ("Four", cache[4]); // 4 6 2 5
    cache.insert(7, "Seven"); // 7 4 6 2
    EXPECT_EQ(4, cache.size());
    EXPECT_FALSE(cache.contains(5));

    cache[2] = "Zwei"; // 2 7 4 6
    cache.insert(8, "Eight"); // 8 2 7 4
    EXPECT_EQ(4, cache.size());
    EXPECT_FALSE(cache.contains(6));

    EXPECT_TRUE(cache.contains(4));
    EXPECT_TRUE(cache.contains(7));
    EXPECT_TRUE(cache.contains(2));
    EXPECT_TRUE(cache.contains(8));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}