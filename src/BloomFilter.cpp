#include <cake/BloomFilter.h>

#include <algorithm>
#include <cmath>

namespace cake {

namespace {
const size_t maxNumBits = (static_cast<size_t>(1) << 35); // Around 4GB
const size_t maxNumHashes = 256;
} // namespace

BloomFilter::BloomFilter(size_t expectedCount, double falsePositiveRate = 0.01)
    : m_expectedCount(expectedCount), m_falsePositiveRate(falsePositiveRate) {
        m_expectedCount = std::max(static_cast<size_t>(1), m_expectedCount);
        m_falsePositiveRate = std::clamp(m_falsePositiveRate, 0.0005, 0.5);

    const double ln_2 = std::log(2.0);
    const double ln2_2 = ln_2 * ln_2;
    const size_t numBits = -static_cast<long long>(m_expectedCount) * log(falsePositiveRate) / ln2_2;
    const size_t numHashes = (numBits / m_expectedCount) * ln_2;

    m_numHashes = std::min(numHashes, maxNumHashes);
    m_bitArray = std::vector<bool>(std::min(numBits, maxNumBits), false);
}

void BloomFilter::clear() {
    for (size_t i = 0; i < m_bitArray.size(); i++)
        m_bitArray[i] = false;
}

double BloomFilter::occupancy() const {
    double occupancy = 0.0;

    for (const auto bit : m_bitArray) {
        occupancy += bit ? 1.0 : 0.0;
    }

    return occupancy / m_bitArray.size();
}
} // namespace cake