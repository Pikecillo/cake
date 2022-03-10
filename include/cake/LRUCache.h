#pragma once

#include <list>
#include <map>
#include <utility>

namespace cake {

/**
 * LRU Cache data structure. The cache keeps a fixed size set of values that can
 * be retrieved through their associated keys. If the cache is full, every time
 * a new entry is inserted, the least recently used entry is removed from the cache
 * to leave space for the new entry.
 */
template <class TKey, class TValue> class LRUCache {
  public:
    using key_type = TKey;
    using value_type = TValue;

  private:
    using KeyValuePair = std::pair<key_type, value_type>;

  public:
    /**
     * Constructor. If given maximum size is zero, an exception is thrown.
     *
     * @param maxSize Maximum size allowed.
     */
    explicit LRUCache(size_t maxSize);

    /**
     * Returns the maximum number of entries allowed in the cache.
     *
     * @return Maximum size allowed.
     */
    size_t maxSize() const { return m_maxSize; }

    /**
     * Returns the number of entries currently in the cache.
     *
     * @return Current size.
     */
    size_t size() const { return m_size; }

    /**
     * Returns a reference to the entry with the given key. This operations touches
     * the entry, so it makes it the most recently used entry.
     * If the entry does not exist, one is created.
     *
     * @param key The given key,
     *
     * @return A reference to the associated value.
     */
    value_type &operator[](const key_type &key);

    /**
     * Insert a new entry into the cache. This operations touches
     * the entry, so it makes it the most recently used entry.
     *
     * @param key The given key
     * @param value The given value
     */
    void insert(const key_type &key, const value_type &value);

    /**
     * Checks if there is an entry with the given key. This operations touches
     * the entry, so it makes it the most recently used entry.
     *
     * @param key The given key
     *
     * @return true if there is an entry in the cache with the given key,
     * false otherwise.
     */
    bool contains(const key_type &key);

    /**
     * Checks if there is an entry with the given key.
     *
     * @param key The given key
     *
     * @return true if an entry was removed from the cache,
     * false otherwise.
     */
    bool remove(const key_type &key);

  private:
    /**
     * Updates the cache such that the entry to which a given iterator becomes
     * the most recently used entry.
     * 
     * @param it A given iterator. 
     */
    void touchEntry(typename std::list<KeyValuePair>::iterator &it);

    /**
     * Evicts the least recently used entry from the cache.
     */
    void evictLRUEntry();

  private:
    std::map<key_type, typename std::list<KeyValuePair>::iterator> m_cache;
    std::list<KeyValuePair> m_entries;
    size_t m_maxSize;
    size_t m_size;
};

template <class TKey, class TValue>
LRUCache<TKey, TValue>::LRUCache(size_t maxSize) : m_maxSize(maxSize), m_size(0) {
    m_maxSize = std::max(static_cast<size_t>(1), m_maxSize);
}

template <class TKey, class TValue>
TValue &LRUCache<TKey, TValue>::operator[](const key_type &key) {
    const auto it = m_cache.find(key);

    if (it != m_cache.end()) {
        touchEntry(it->second);
    } else {
        insert(key, value_type());
    }

    return m_entries.front().second;
}

template <class TKey, class TValue>
void LRUCache<TKey, TValue>::insert(const key_type &key, const value_type &value) {
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

template <class TKey, class TValue> bool LRUCache<TKey, TValue>::contains(const key_type &key) {
    const auto it = m_cache.find(key);

    if (it == m_cache.end()) {
        return false;
    }

    touchEntry(it->second);

    return true;
}

template <class TKey, class TValue> bool LRUCache<TKey, TValue>::remove(const key_type &key) {
    const auto it = m_cache.find(key);

    if (it == m_cache.end())
        return false;

    --m_size;
    m_entries.erase(it->second);
    m_cache.erase(it);

    return true;
}

template <class TKey, class TValue>
void LRUCache<TKey, TValue>::touchEntry(typename std::list<KeyValuePair>::iterator &it) {
    const auto &[key, value] = *it;
    m_entries.push_front(std::make_pair(key, value));
    m_entries.erase(it);
    m_cache[key] = m_entries.begin();
}

template <class TKey, class TValue> void LRUCache<TKey, TValue>::evictLRUEntry() {
    const auto lru = m_entries.back();
    m_cache.erase(lru.first);
    m_entries.pop_back();
}
} // namespace cake