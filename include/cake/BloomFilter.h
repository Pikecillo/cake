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
    BloomFilter(size_t expectedCount, double falsePositiveRate = 0.01)
        : m_expectedCount(expectedCount)
        , m_falsePositiveRate(falsePositiveRate)
    {
        const double ln_2 = std::log(2.0);
        const double ln2_2 = ln_2 * ln_2;
        const size_t size = -static_cast<int>(expectedCount) * log(falsePositiveRate) / ln2_2;
        const size_t numHashes = static_cast<size_t>(size / expectedCount * ln_2);
        const size_t maxSize = (1 << 20);
        const size_t maxNumHashes = 8;

        m_numHashes = std::max(numHashes, maxNumHashes);
        m_bitArray = std::vector<bool>(std::min(size, maxSize), false);
    }

    template <typename Type>
    bool contains(const Type& element) const
    {
        const auto hash_result = Hash::md5(&element, sizeof(Type));

        for (size_t i = 0; i < m_numHashes; ++i) {
            const size_t idx = ((hash_result[2 * i] << 8) | hash_result[2 * i + 1]) % m_bitArray.size();
            if (!m_bitArray[idx])
                return false;
        }

        return true;
    }

    template <typename Type>
    const void add(const Type& element)
    {
        const auto hash_result = Hash::md5(&element, sizeof(Type));

        for (size_t i = 0; i < m_numHashes; ++i) {
            const size_t idx = ((hash_result[2 * i] << 8) | hash_result[2 * i + 1]) % m_bitArray.size();
            m_bitArray[idx] = true;
        }
    }

    void clear();

    double occupancy() const;

private:
    size_t m_expectedCount;
    double m_falsePositiveRate;
    size_t m_size, m_numHashes;
    std::vector<bool> m_bitArray;
};
} // namespace cake