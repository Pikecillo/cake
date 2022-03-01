#include <gtest/gtest.h>

#include <cake/Trie.h>

TEST(TrieTest, constructEmpty) {
    cake::Trie trie;

    ASSERT_EQ(trie.size(), 0);
}

TEST(TrieTest, constructWithWordContainer) {
    std::vector<std::string> words = {"apple", "brave", "application", "cake", "cat"};
    cake::Trie trie(words);

    ASSERT_EQ(trie.size(), words.size());
}

TEST(TrieTest, addWord) {
    cake::Trie trie;

    ASSERT_EQ(trie.size(), 0);

    trie.addWord("cake");

    ASSERT_EQ(trie.size(), 1);
}

TEST(TrieTest, query) {
    std::vector<std::string> words = {"apple", "brave", "application", "cake", "cat"};
    cake::Trie trie(words);

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