#pragma once

namespace cake {
    template <typename T>
    class BloomFilter {
        public:

        bool contains(const T& element) const {
            return false;
        }

        const void add(const T& element) {}
    };
}