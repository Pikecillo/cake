/**
 * MIT License
 * 
 * Copyright (c) 2022 Mario Rincon Nigro
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cake/BloomFilter.h>

#include <algorithm>
#include <cmath>

namespace cake {

namespace {
const size_t maxNumBits = (static_cast<size_t>(1) << 35); // Around 4GB
const size_t maxNumHashes = 256;
} // namespace

BloomFilter::BloomFilter(size_t expectedNumElements, double falsePositiveRate = 0.01)
    : m_expectedNumElements(expectedNumElements), m_falsePositiveRate(falsePositiveRate) {
    m_expectedNumElements = std::max(static_cast<size_t>(1), m_expectedNumElements);
    m_falsePositiveRate = std::clamp(m_falsePositiveRate, 0.0005, 0.5);

    const double ln_2 = std::log(2.0);
    const double ln2_2 = ln_2 * ln_2;
    const size_t numBits =
        -static_cast<long long>(m_expectedNumElements) * log(falsePositiveRate) / ln2_2;
    const size_t numHashes = (numBits / m_expectedNumElements) * ln_2;

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