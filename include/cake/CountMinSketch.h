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

#include <limits>
#include <vector>

namespace cake {
    class CountMinSketch {
        public:
            CountMinSketch(size_t expectedNumElements) {
                // TODO initialize the cells
            }

            template <typename TElement>
            void increment(const TElement& element) {
                const auto idxs = computeElementIndices(element);

                for(size_t i = 0; i < idxs.size(); ++i) {
                    m_counts[i][idxs[i]]++;
                }
            }

            template <typename TElement>
            size_t count(const TElement& element) const {
                size_t estimateCount = std::numeric_limits<size_t>::max();
                const auto idxs = computeElementIndices(element);

                for(size_t i = 0; i < idxs.size(); ++i) {
                    const size_t count = m_counts[i][idxs[i]];

                    if(estimateCount > count) estimateCount = count;
                }

                return estimateCount;
            }

        private:
            template <typename TElement>
            std::vector<size_t> computeElementIndices(const TElement& element) const {
                std::vector<size_t> idxs;
                const size_t numCols = m_counts.back().size();

                for(int i = 0; i < m_counts.size(); ++i) {
                    const size_t idx = Hash::murmur64A(element, numCols * i) % numCols;
                    idxs.push_back(idx);
                }

                return idxs;
            }

        private:
            std::vector<std::vector<size_t>> m_counts;
    };
}