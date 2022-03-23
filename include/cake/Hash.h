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

/**
 * Computes 64bit MurmurHash2 of a given object. The object must be of fundamental type.
 *
 * @param object Given object.
 * @param seed 'Random' value to use as seed of the hash.
 *
 * @return 64 bit hash.
 */
template <typename TObject,
          typename std::enable_if_t<std::is_fundamental<TObject>::value, int> = 0>
          uint64_t murmur64A(const TObject object, uint64_t seed) {
    return murmur64A(&object, sizeof(TObject), seed);
}

/**
 * Computes 64bit MurmurHash2 of a given string.
 *
 * @param object Reference to string.
 * @param seed 'Random' value to use as seed of the hash.
 *
 * @return 64 bit hash.
 */
template <typename TChar> uint64_t murmur64A(const std::basic_string<TChar> &str, uint64_t seed) {
    return murmur64A(str.data(), str.size() * sizeof(TChar), seed);
}

/**
 * Computes 64bit MurmurHash2 of a vector of objects of fundamental type.
 *
 * @param object Reference to vector.
 * @param seed 'Random' value to use as seed of the hash.
 *
 * @return 64 bit hash.
 */
template <typename TElement,
          typename std::enable_if_t<std::is_fundamental<TElement>::value, int> = 0>
uint64_t murmur64A(const std::vector<TElement> &vec, uint64_t seed) {
    return murmur64A(vec.data(), vec.size() * sizeof(TElement), seed);
}
} // namespace Hash
} // namespace cake