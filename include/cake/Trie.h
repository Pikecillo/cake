#pragma once

#include <string>
#include <map>
#include <vector>

namespace cake {

class Trie {
    private:
        struct Node {
            bool isWordEnd;
            std::map<char, Node*> children;

            Node() {
                isWordEnd = false;
            }

            ~Node() {
               // destroy(this);
            }

            void destroy(Node* node) {
                if(node == nullptr) return;

                for(auto& [symbol, child]: node->children) {
                    destroy(child);
                    delete child;
                }
            }
        };

    public:
        Trie(): m_size(0) {}

        Trie(const std::vector<std::string>& words): m_size(0) {
            for(const auto& word: words) {
                addWord(word);
            }
        }

        ~Trie() {
           for(auto [_, child]: m_children)
                delete child;
        }

        void addWord(const std::string& word) {
            if(word.empty()) return;

            Node* node = nullptr;
            auto it = m_children.find(word[0]);

            if(it == m_children.end()) {
                node = new Node();
                m_children[word[0]] = node;
            } else {
                node = it->second;
            }

            for(char c: word.substr(1)) {
                it = node->children.find(c);
                if(it == node->children.end()) {
                    Node* newNode = new Node();
                    node->children[c] = newNode;
                }

                node = node->children[c];
            }

            node->isWordEnd = true;
            m_size++;
        }

        size_t size() const {
            return m_size;
        }

        std::vector<std::string> query(const std::string& prefix) const {
            std::vector<std::string> result;

            auto it = m_children.find(prefix[0]);

            if(it == m_children.end()) return result;

            Node *node = it->second;

            for(char c: prefix.substr(1)) {
                if(node->children.count(c) == 0)
                    return result;

                node = node->children.at(c);
            }

            query(node, prefix, "", result);
            
            return result;
        }

        void query(const Node* node, const std::string& prefix, std::string word, std::vector<std::string>& result) const {
            if(node == nullptr) {
                return;
            }

            for(auto [symbol, child]: node->children) {
                if(child != nullptr && child->isWordEnd)
                    result.push_back(prefix + word + symbol);
                
                query(child, prefix, word + symbol, result);
            }
        }

    private:
        std::map<char, Node*> m_children;
        size_t m_size;
};
}