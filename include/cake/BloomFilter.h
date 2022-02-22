#pragma once

#include <array>
#include <vector>

#include <cake/Hash.h>

namespace cake {
    class BloomFilter {
        public:
            BloomFilter(size_t size, size_t numHashes) : m_numHashes(numHashes), m_bitArray(size, false) {}

        template <typename Type>
        bool contains(const Type& element) const {
            const auto hash_result = Hash::md5(&element, sizeof(Type));

            for(size_t i = 0; i < m_numHashes; ++i) {
                if(!m_bitArray[hash_result[i]])
                    return false;
            }

            return true;
        }

        template <typename Type>
        const void add(const Type& element) {
            const auto hash_result = Hash::md5(&element, sizeof(Type));

            for(size_t i = 0; i < m_numHashes; ++i) {
                m_bitArray[hash_result[i]] = true;
            }
        }

        void clear() {
            for(size_t i = 0; i < m_bitArray.size(); i++)
                m_bitArray[i] = false;
        }

        double occupancy() const {
            double occupancy = 0.0;

            for(const auto bit: m_bitArray) {
                occupancy += bit;
            }

            return occupancy / m_bitArray.size();
        }

        private:
            size_t m_size, m_numHashes;
            std::vector<bool> m_bitArray;
    };
}