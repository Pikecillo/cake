#include <cake/LRUCache.h>

#include <set>
#include <string>

namespace cake {
template class LRUCache<int, std::string>;
template class LRUCache<int, std::set<int>>;
} // namespace cake