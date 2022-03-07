#include <cake/Hash.h>

#include <openssl/md5.h>

namespace cake {
namespace Hash {
    Result md5(const void* data, size_t size)
    {
        Result result;
        MD5(reinterpret_cast<const unsigned char*>(data), size, result.data());
        return result;
    };
} // namespace Hash
} // namespace cake