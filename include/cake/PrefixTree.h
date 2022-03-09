#pragma once

#include <map>
#include <memory>
#include <stack>
#include <vector>

namespace cake {

/**
 * Prefix Tree data structure. Allows for quick retrieval of
 * all words that match a given prefix among a given set of words.
 */
template <typename TString> class PrefixTree {
  public:
    using string_type = TString;
    using symbol_type = typename TString::value_type;

    /**
     * Constructor. Creates an empty prefix tree.
     */
    PrefixTree();

    /**
     * Constructor. Creates a prefix tree with all the words in a given vector.
     *
     * @param words Container of words.
     */
    PrefixTree(const std::vector<string_type> &words);

    /** Returns how many words the prefix tree contains.
     *
     * @return The number of words in the prefix tree.
     */
    size_t size() const { return m_size; }

    /**
     * Add a new word to the prefix tree, if the word was not already present.
     *
     * @param word Given word.
     *
     * @return true if the new word is not empty and was not already present, otherwise false.
     */
    bool add(const string_type &word);

    /**
     * Remove a word from the prefix tree.
     *
     * @param word Given word.
     *
     * @return true if the word was removed, otherwise false.
     */
    bool remove(const string_type &word);

    /**
     * Query prefix tree for all words that match a given prefix.
     *
     * @param prefix A given prefix.
     *
     * @return A vector with all the words in the prefix tree that match the given prefix.
     */
    std::vector<string_type> query(const string_type &prefix) const;

  private:
    struct Node;

    std::unique_ptr<Node> m_root;
    size_t m_size;
};

template <typename TString> struct PrefixTree<TString>::Node {
    bool isWordEnd;
    std::map<symbol_type, std::unique_ptr<Node>> children;

    Node() : isWordEnd(false) {}

    std::vector<string_type> collect(const string_type &prefix) const {
        std::vector<string_type> result;

        collect(prefix, string_type(), result);

        return result;
    }

  private:
    void collect(const string_type &prefix, const string_type &wordEnding,
                 std::vector<string_type> &result) const {
        if (isWordEnd) {
            result.push_back(prefix + wordEnding);
        }

        for (const auto &[symbol, child] : children) {
            child->collect(prefix, wordEnding + symbol, result);
        }
    }
};

template <typename TString>
PrefixTree<TString>::PrefixTree() : m_root(std::make_unique<Node>()), m_size(0) {}

template <typename TString>
PrefixTree<TString>::PrefixTree(const std::vector<string_type> &words)
    : m_root(std::make_unique<Node>()), m_size(0) {
    for (const auto &word : words) {
        add(word);
    }
}

template <typename TString> bool PrefixTree<TString>::add(const string_type &word) {
    if (word.empty())
        return false;

    Node *node = m_root.get();

    for (const symbol_type symbol : word) {
        const auto it = node->children.find(symbol);

        if (it == node->children.end())
            node->children[symbol] = std::make_unique<Node>();

        node = node->children[symbol].get();
    }

    if (node->isWordEnd)
        return false;

    ++m_size;
    node->isWordEnd = true;

    return true;
}

template <typename TString> bool PrefixTree<TString>::remove(const string_type &word) {
    if (word.empty())
        return false;

    std::stack<std::pair<symbol_type, Node *>> path;
    Node *node = m_root.get();

    for (const symbol_type symbol : word) {
        const auto it = node->children.find(symbol);

        if (it == node->children.end() || it->second == nullptr)
            return false;

        path.push(std::make_pair(symbol, node));

        node = it->second.get();
    }

    if (!node->isWordEnd)
        return false;

    if (!node->children.empty()) {
        node->isWordEnd = false;
    } else {
        while (!path.empty()) {
            auto [symbol, node] = path.top();

            path.pop();
            node->children.erase(symbol);

            if (node->isWordEnd)
                break;
        }
    }

    --m_size;

    return true;
}

template <typename TString>
std::vector<TString> PrefixTree<TString>::query(const string_type &prefix) const {
    if (prefix.empty())
        return {};

    Node *node = m_root.get();

    for (const symbol_type symbol : prefix) {
        const auto it = node->children.find(symbol);

        if (it == node->children.end() || it->second == nullptr)
            return {};

        node = it->second.get();
    }

    return node->collect(prefix);
}

} // namespace cake