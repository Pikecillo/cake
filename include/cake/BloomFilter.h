#pragma once

#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <vector>

#include <cake/Hash.h>

namespace cake {

/**
 * Bloom Filter data structure. The Bloom Filter allows for approximate testing
 * of membership to a set, allowing for a certain rate of false positives to occur.
 * That is, the membership test to the set through the bloom filter may return
 * "true", meaning "possibly in set", and "false" meaning "not in set".
 */
class BloomFilter {
  public:
    /**
     * Constructor.
     *
     * @param expectedCount Number of expected elements to be added to the set.
     * @param falsePositiveRate Desired false positive rate.
     */
    BloomFilter(size_t expectedCount, double falsePositiveRate);

    template <typename Type> bool contains(const Type &element) const;

    template <typename Type> void add(const Type &element);

    /**
     * Clears the underlying set
     */
    void clear();

    /**
     * Computes the occupancy of the filter. This is a measure of how "full"
     * the filter is.
     *
     * @return A value in the range [0.0, 1.0] representing how full the filter is.
     */
    double occupancy() const;

  private:
    size_t m_expectedCount;
    double m_falsePositiveRate;
    size_t m_size, m_numHashes;
    std::vector<bool> m_bitArray;
};

/**
 * Test an element for membership into the underlying set. The test allows for a
 * certain rate of false positives to occur.
 *
 * @param element The given element.
 *
 * @return true, when the element is possibly in the set; false, when the element is
 * guaranteed not to be in the set.
 */
template <typename TElement> bool BloomFilter::contains(const TElement &element) const {
    std::array<unsigned char, sizeof(TElement) + 1> data;
    std::memcpy(data.data() + 1, &element, sizeof(TElement));

    for (size_t i = 1; i <= m_numHashes; ++i) {
        data[0] = static_cast<unsigned char>(i);
        const auto hash_result = Hash::md5(data.data(), data.size());

        size_t idx = 0;
        for (size_t j = 0; j < sizeof(size_t); ++j) {
            idx = ((idx << 8) | hash_result[j]);
        }

        idx = idx % m_bitArray.size();

        if (!m_bitArray[idx])
            return false;
    }

    return true;
}

/**
 * Add a new element to the underlying set.
 *
 * @param The element to be added.
 */
template <typename TElement> void BloomFilter::add(const TElement &element) {
    std::array<unsigned char, sizeof(TElement) + 1> data;
    std::memcpy(data.data() + 1, &element, sizeof(TElement));

    for (size_t i = 1; i <= m_numHashes; ++i) {
        data[0] = static_cast<unsigned char>(i);
        const auto hash_result = Hash::md5(data.data(), data.size());

        size_t idx = 0;
        for (size_t j = 0; j < sizeof(size_t); ++j) {
            idx = ((idx << 8) | hash_result[j]);
        }

        idx = idx % m_bitArray.size();
        m_bitArray[idx] = true;
    }
}
} // namespace cake