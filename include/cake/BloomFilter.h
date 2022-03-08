#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>

#include <cake/Hash.h>

namespace cake {
class BloomFilter {
  public:
    BloomFilter(size_t expectedCount, double falsePositiveRate);

    template <typename Type> bool contains(const Type &element) const;

    template <typename Type> const void add(const Type &element);

    void clear();

    double occupancy() const;

  private:
    size_t m_expectedCount;
    double m_falsePositiveRate;
    size_t m_size, m_numHashes;
    std::vector<bool> m_bitArray;
};

template <typename TElement> bool BloomFilter::contains(const TElement &element) const {
    const auto hash_result = Hash::md5(&element, sizeof(TElement));

    for (size_t i = 0; i < m_numHashes; ++i) {
        const size_t idx = ((hash_result[2 * i] << 8) | hash_result[2 * i + 1]) % m_bitArray.size();
        if (!m_bitArray[idx])
            return false;
    }

    return true;
}

template <typename TElement> const void BloomFilter::add(const TElement &element) {
    const auto hash_result = Hash::md5(&element, sizeof(TElement));

    for (size_t i = 0; i < m_numHashes; ++i) {
        const size_t idx = ((hash_result[2 * i] << 8) | hash_result[2 * i + 1]) % m_bitArray.size();
        m_bitArray[idx] = true;
    }
}
} // namespace cake