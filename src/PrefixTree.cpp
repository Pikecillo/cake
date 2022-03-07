#include <cake/PrefixTree.h>

#include <map>
#include <stack>

namespace cake {

struct PrefixTree::Node {
    bool isWordEnd;
    std::map<char, std::unique_ptr<Node>> children;

    Node() : isWordEnd(false)
    {
    }

    std::vector<std::string> collect(const std::string& prefix) const
    {
        std::vector<std::string> result;

        collect(prefix, std::string(), result);

        return result;
    }

private:
    void collect(const std::string& prefix, const std::string& wordEnding,
        std::vector<std::string>& result) const
    {
        if (isWordEnd) {
            result.push_back(prefix + wordEnding);
        }

        for (const auto& [symbol, child] : children) {
            child->collect(prefix, wordEnding + symbol, result);
        }
    }
};

PrefixTree::PrefixTree()
    : m_root(std::make_unique<Node>())
    , m_size(0)
{
}

PrefixTree::PrefixTree(const std::vector<std::string>& words)
    : m_root(std::make_unique<Node>())
    , m_size(0)
{
    for (const auto& word : words) {
        add(word);
    }
}

PrefixTree::~PrefixTree() = default;

bool PrefixTree::add(const std::string& word)
{
    if (word.empty())
        return false;

    Node* node = m_root.get();

    for (const char symbol : word) {
        const auto it = node->children.find(symbol);

        if (it == node->children.end()) {
            Node* newNode = new Node();
            node->children[symbol] = std::make_unique<Node>();
        }

        node = node->children[symbol].get();
    }

    if(node->isWordEnd)
        return false;

    ++m_size;
    node->isWordEnd = true;

    return true;
}

bool PrefixTree::remove(const std::string& word)
{
    if(word.empty())
        return false;

    std::stack<std::pair<char, Node*>> path;
    Node* node = m_root.get();

    for (const char symbol : word) {
        const auto it = node->children.find(symbol);

        if (it == node->children.end() || it->second == nullptr)
            return false;

        path.push(std::make_pair(symbol, node));

        node = it->second.get();
    }

    if(!node->isWordEnd) return false;

    if(!node->children.empty()) {
        node->isWordEnd = false;
    }
    else {
        while(!path.empty()) {
            auto [symbol, node] = path.top();
            
            path.pop();
            node->children.erase(symbol);

            if(node->isWordEnd) break;
        }
    }

    --m_size;

    return true;
}

std::vector<std::string> PrefixTree::query(const std::string& prefix) const
{
    if(prefix.empty())
        return {};

    Node* node = m_root.get();

    for (const char symbol : prefix) {
        const auto it = node->children.find(symbol);

        if (it == node->children.end() || it->second == nullptr)
            return {};

        node = it->second.get();
    }

    return node->collect(prefix);
}
} // namespace cake