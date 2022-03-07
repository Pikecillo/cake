#pragma once

#include <array>

namespace cake {
namespace Hash {
using Result = std::array<unsigned char, 16>;

Result md5(const void *data, size_t size);
} // namespace Hash
} // namespace cake