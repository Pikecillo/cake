#pragma once

#include <list>
#include <map>
#include <utility>

namespace cake {
template <class TKey, class TValue> class LRUCache {
  public:
    using key_type = TKey;
    using value_type = TValue;

  private:
    using KeyValuePair = std::pair<key_type, value_type>;

  public:
    LRUCache(size_t maxSize) : m_maxSize(maxSize), m_size(0) {}

    size_t maxSize() const { return m_maxSize; }
    size_t size() const { return m_size; }

    value_type &operator[](const key_type &key) {
        const auto it = m_cache.find(key);

        if (it != m_cache.end()) {
            touchEntry(it->second);
        } else {
            insert(key, value_type());
        }

        return m_entries.front().second;
    }

    void insert(const key_type &key, const value_type &value) {
        const auto it = m_cache.find(key);

        if (it != m_cache.end()) {
            m_entries.erase(it->second);
        } else if (m_size == m_maxSize) {
            evictLRUEntry();
        } else {
            ++m_size;
        }

        m_entries.push_front(std::make_pair(key, value));
        m_cache[key] = m_entries.begin();
    }

    bool contains(const key_type &key) {
        const auto it = m_cache.find(key);

        if (it == m_cache.end()) {
            return false;
        }

        touchEntry(it->second);

        return true;
    }

    bool remove(const key_type &key) {
        const auto it = m_cache.find(key);

        if (it == m_cache.end())
            return false;

        --m_size;
        m_entries.erase(it->second);
        m_cache.erase(it);

        return true;
    }

  private:
    void touchEntry(typename std::list<KeyValuePair>::iterator &it) {
        const auto &[key, value] = *it;
        m_entries.push_front(std::make_pair(key, value));
        m_entries.erase(it);
        m_cache[key] = m_entries.begin();
    }

    void evictLRUEntry() {
        const auto lru = m_entries.back();
        m_cache.erase(lru.first);
        m_entries.pop_back();
    }

  private:
    std::map<key_type, typename std::list<KeyValuePair>::iterator> m_cache;
    std::list<KeyValuePair> m_entries;
    size_t m_maxSize;
    size_t m_size;
};
} // namespace cake