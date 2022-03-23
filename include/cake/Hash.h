#pragma once

#include <string>
#include <vector>

namespace cake {
namespace Hash {

/**
 * Computes 64bit MurmurHash2 of given data.
 *
 * @param data Pointer to data.
 * @param sizeInBytes Size in bytes of data.
 * @param seed 'Random' value to use as seed of the hash.
 *
 * @return 64 bit hash.
 */
uint64_t murmur64A(const void *data, size_t sizeInBytes, uint64_t seed);

template <typename TObject> uint64_t murmur64A(const TObject &object, uint64_t seed) {
    return murmur64A(&object, sizeof(TObject), seed);
}

template <typename TChar> uint64_t murmur64A(const std::basic_string<TChar> &str, uint64_t seed) {
    return murmur64A(str.data(), str.size() * sizeof(TChar), seed);
}

template <typename TElement,
          typename std::enable_if_t<std::is_fundamental<TElement>::value, int> = 0>
uint64_t murmur64A(const std::vector<TElement> &vec, uint64_t seed) {
    return murmur64A(vec.data(), vec.size() * sizeof(TElement), seed);
}
} // namespace Hash
} // namespace cake