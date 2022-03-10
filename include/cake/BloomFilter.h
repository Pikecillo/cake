#pragma once

#include <algorithm>
#include <array>
#include <cstring>
#include <vector>

#include <cake/Hash.h>

namespace cake {

/**
 * Bloom Filter data structure. The Bloom Filter allows for approximate testing
 * of membership to a set, allowing for a certain rate of false positives to occur.
 * That is, the membership test through the bloom filter may return "true", meaning "possibly in
 * set", and "false" meaning "not in set".
 */
class BloomFilter {
  public:
    /**
     * Constructor.
     *
     * @param expectedCount Number of expected elements to be added to the set. The value is not allower
     * to be 0 (a value of 0 will be converted to 1).
     * @param falsePositiveRate Desired false positive rate. Values are clamped to the interval
     * [0.0005, 0.5]
     */
    BloomFilter(size_t expectedCount, double falsePositiveRate);

    /**
     * Returns the given expected number of elements to be added as specified in constructor parameter.
     */
    size_t expectedCount() const { return m_expectedCount; }

    /**
     * Returns the desired false positive rate as specified in constructor parameter.
     */
    double falsePositiveRate() const { return m_falsePositiveRate; };

    /**
     * Test an element for membership into the underlying set. The test allows for a
     * certain rate of false positives to occur.
     *
     * @param element The given element.
     *
     * @return true, when the element is possibly in the set; false, when the element is
     * guaranteed not to be in the set.
     */
    template <typename TElement> bool contains(const TElement &element) const;

    /**
     * Add a new element to the underlying set.
     *
     * @param The element to be added.
     */
    template <typename TElement> void add(const TElement &element);

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
    /**
     * Computes the bit index, for the given data
     *
     * @param data An array of bytes. This is an MD5 has with an additional byte concatenated.
     * The additional byte represent the id of the hash operation.
     *
     * @return The bit index.
     */
    template <size_t Size> size_t computeIdx(const std::array<unsigned char, Size> &data) const;

  private:
    size_t m_expectedCount;
    double m_falsePositiveRate;
    size_t m_size, m_numHashes;
    std::vector<bool> m_bitArray;
};

template <size_t Size>
size_t BloomFilter::computeIdx(const std::array<unsigned char, Size> &data) const {
    const auto hash_result = Hash::md5(data.data(), data.size());

    size_t idx = 0;
    for (size_t j = 0; j < sizeof(size_t); ++j) {
        idx = ((idx << 8) | hash_result[j]);
    }

    idx = idx % m_bitArray.size();

    return idx;
}

template <typename TElement> bool BloomFilter::contains(const TElement &element) const {
    std::array<unsigned char, sizeof(TElement) + 1> data;
    std::memcpy(data.data() + 1, &element, sizeof(TElement));

    for (size_t i = 1; i <= m_numHashes; ++i) {
        data[0] = static_cast<unsigned char>(i);

        const size_t idx = computeIdx(data);

        if (!m_bitArray[idx])
            return false;
    }

    return true;
}

template <typename TElement> void BloomFilter::add(const TElement &element) {
    std::array<unsigned char, sizeof(TElement) + 1> data;
    std::memcpy(data.data() + 1, &element, sizeof(TElement));

    for (size_t i = 1; i <= m_numHashes; ++i) {
        data[0] = static_cast<unsigned char>(i);

        const size_t idx = computeIdx(data);

        m_bitArray[idx] = true;
    }
}
} // namespace cake