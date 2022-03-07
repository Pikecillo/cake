#include <cake/BloomFilter.h>

namespace cake {

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