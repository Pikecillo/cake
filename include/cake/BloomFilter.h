#pragma once

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
     * @param expectedCount Number of expected elements to be added to the set. The value is not
     * allowed to be 0 (a value of 0 will be converted to 1).
     * @param falsePositiveRate Desired false positive rate. Values are clamped to the interval
     * [0.0005, 0.5]
     */
    BloomFilter(size_t expectedCount, double falsePositiveRate);

    /**
     * Returns the given expected number of elements to be added as specified in constructor
     * parameter.
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

    /**
     * Return the size of the filter
     *
     * @return Size of the filter
     */
    size_t size() const { return m_bitArray.size(); }

  private:
    /**
     * Given an object, computes its indices in the bitmap.
     *
     * @param element The given element.
     *
     * @return The indices of the element.
     */
    template <typename TElement>
    std::vector<size_t> computeElementIndices(const TElement &element) const;

  private:
    size_t m_expectedCount;
    double m_falsePositiveRate;
    size_t m_numHashes;
    std::vector<bool> m_bitArray;
};

template <typename TElement>
std::vector<size_t> BloomFilter::computeElementIndices(const TElement &element) const {
    std::vector<size_t> idxs;

    for (size_t i = 1; i <= m_numHashes; ++i) {
        const size_t idx = Hash::murmur64A(element, m_bitArray.size() * i) % m_bitArray.size();
        idxs.push_back(idx);
    }

    return idxs;
}

template <typename TElement> bool BloomFilter::contains(const TElement &element) const {
    const auto indices = computeElementIndices(element);

    for (const auto idx : indices) {
        if (!m_bitArray[idx])
            return false;
    }

    return true;
}

template <typename TElement> void BloomFilter::add(const TElement &element) {
    const auto indices = computeElementIndices(element);

    for (const auto idx : indices) {
        m_bitArray[idx] = true;
    }
}
} // namespace cake