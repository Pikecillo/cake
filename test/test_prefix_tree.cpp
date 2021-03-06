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

#include <string>

#include <gtest/gtest.h>

#include <cake/PrefixTree.h>

TEST(PrefixTreeTest, constructEmpty) {
    cake::PrefixTree<std::string> trie;

    EXPECT_EQ(trie.size(), 0);
}

TEST(PrefixTreeTest, constructWithWordContainer) {
    {
        std::vector<std::string> words = {"apple", "brave", "application", "cake", "cat"};
        cake::PrefixTree<std::string> trie(words);

        EXPECT_EQ(trie.size(), words.size());
    }
    {
        std::vector<std::string> words = {"apple", "apple", "brave", "application",
                                          "cake",  "cat",   ""};
        cake::PrefixTree<std::string> trie(words);

        ASSERT_EQ(trie.size(), 5);
    }
}

TEST(PrefixTreeTest, add) {
    cake::PrefixTree<std::string> trie;

    EXPECT_FALSE(trie.add(""));
    EXPECT_EQ(trie.size(), 0);

    EXPECT_TRUE(trie.add("cakes"));
    EXPECT_EQ(trie.size(), 1);

    EXPECT_FALSE(trie.add("cakes"));
    EXPECT_EQ(trie.size(), 1);

    EXPECT_TRUE(trie.add("cake"));
    EXPECT_EQ(trie.size(), 2);

    EXPECT_TRUE(trie.add("dude"));
    EXPECT_EQ(trie.size(), 3);

    EXPECT_TRUE(trie.add("duderino"));
    EXPECT_EQ(trie.size(), 4);
}

TEST(PrefixTreeTest, remove) {
    std::vector<std::string> words = {"beat", "cup", "cupcakes", "cupcake", "dude", "duderino"};
    cake::PrefixTree<std::string> trie(words);

    EXPECT_FALSE(trie.remove(""));
    EXPECT_EQ(trie.size(), 6);

    EXPECT_FALSE(trie.remove("beta"));
    EXPECT_EQ(trie.size(), 6);

    EXPECT_FALSE(trie.remove("beats"));
    EXPECT_EQ(trie.size(), 6);

    EXPECT_TRUE(trie.remove("beat"));
    EXPECT_EQ(trie.size(), 5);

    EXPECT_TRUE(trie.remove("cup"));
    EXPECT_EQ(trie.size(), 4);

    EXPECT_TRUE(trie.remove("cupcake"));
    EXPECT_EQ(trie.size(), 3);

    EXPECT_TRUE(trie.remove("duderino"));
    EXPECT_EQ(trie.size(), 2);

    EXPECT_FALSE(trie.remove("duderino"));
    EXPECT_EQ(trie.size(), 2);

    EXPECT_FALSE(trie.add("dude"));
    EXPECT_EQ(trie.size(), 2);
}

TEST(PrefixTreeTest, query) {
    std::vector<std::string> words = {"apple", "brave", "application", "cake", "cat"};
    cake::PrefixTree<std::string> trie(words);

    {
        const std::vector<std::string> expected = {"apple", "application"};
        const auto actual = trie.query("app");
        ASSERT_EQ(expected.size(), actual.size());

        EXPECT_EQ(expected[0], actual[0]);
        EXPECT_EQ(expected[1], actual[1]);
    }
    {
        const auto actual = trie.query("");
        EXPECT_TRUE(actual.empty());
    }
    {
        const auto actual = trie.query("applications");
        EXPECT_TRUE(actual.empty());
    }
    {
        const auto actual = trie.query("brave");
        ASSERT_EQ(actual.size(), 1);
        EXPECT_EQ(actual[0], "brave");
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}