#include <cake/Hash.h>

#include "MurmurHash2.h"

namespace cake {
namespace Hash {
uint64_t murmur64A(const void *data, size_t sizeInBytes, uint64_t seed) {
    return MurmurHash64A(data, static_cast<int>(sizeInBytes), seed);
};
} // namespace Hash
} // namespace cake