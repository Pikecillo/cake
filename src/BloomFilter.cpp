#include <cake/BloomFilter.h>

namespace cake {

BloomFilter::BloomFilter(size_t expectedCount, double falsePositiveRate = 0.01)
    : m_expectedCount(expectedCount), m_falsePositiveRate(falsePositiveRate) {
    const double ln_2 = std::log(2.0);
    const double ln2_2 = ln_2 * ln_2;
    const size_t size = -static_cast<int>(expectedCount) * log(falsePositiveRate) / ln2_2;
    const size_t numHashes = static_cast<size_t>(size / expectedCount * ln_2);
    const size_t maxSize = (1 << 20);
    const size_t maxNumHashes = 8;

    m_numHashes = std::max(numHashes, maxNumHashes);
    m_bitArray = std::vector<bool>(std::min(size, maxSize), false);
}

void BloomFilter::clear() {
    for (size_t i = 0; i < m_bitArray.size(); i++)
        m_bitArray[i] = false;
}

double BloomFilter::occupancy() const {
    double occupancy = 0.0;

    for (const auto bit : m_bitArray) {
        occupancy += bit;
    }

    return occupancy / m_bitArray.size();
}
} // namespace cake