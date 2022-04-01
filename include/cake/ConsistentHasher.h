#include <set>

#include <cake/Hash.h>

namespace cake {
class ConsistentHasher {
    ConsistentHasher(size_t rangeSize) {}
    
    size_t hash(size_t value) const {
        const auto hashedValue = Hash::md5(*value, sizeof(value));
    }

    size_t rangeSize() { return m_rangeSize; }

    private:
        size_t m_rangeSize;
};
}