#pragma once

#include <memory>
#include <string>
#include <vector>

namespace cake {

class PrefixTree {
private:
    struct Node;

public:
    PrefixTree();

    PrefixTree(const std::vector<std::string>& words);

    ~PrefixTree();

    size_t size() const
    {
        return m_size;
    }

    bool add(const std::string& word);

    bool remove(const std::string& word);

    std::vector<std::string> query(const std::string& prefix) const;

private:
    std::unique_ptr<Node> m_root;
    size_t m_size;
};

} // namespace cake