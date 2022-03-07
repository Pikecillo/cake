#include <gtest/gtest.h>

#include <cake/PrefixTree.h>

TEST(PrefixTreeTest, constructEmpty) {
    cake::PrefixTree trie;

    ASSERT_EQ(trie.size(), 0);
}

TEST(PrefixTreeTest, constructWithWordContainer) {
    std::vector<std::string> words = {"apple", "brave", "application", "cake", "cat"};
    cake::PrefixTree trie(words);

    ASSERT_EQ(trie.size(), words.size());
}

TEST(PrefixTreeTest, addWord) {
    cake::PrefixTree trie;

    ASSERT_EQ(trie.size(), 0);

    trie.add("cake");

    ASSERT_EQ(trie.size(), 1);
}

TEST(PrefixTreeTest, query) {
    std::vector<std::string> words = {"apple", "brave", "application", "cake", "cat"};
    cake::PrefixTree trie(words);

    {
        const std::vector<std::string> expected = {"apple", "application"};
        const auto actual = trie.query("app");
        EXPECT_EQ(expected.size(), actual.size());

        EXPECT_EQ(expected[0], actual[0]);
        EXPECT_EQ(expected[1], actual[1]);
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}