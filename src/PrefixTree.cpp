#include <cake/PrefixTree.h>

#include <string>

namespace cake {

template class PrefixTree<std::string>;
template class PrefixTree<std::wstring>;
template class PrefixTree<std::u16string>;
template class PrefixTree<std::u32string>;

} // namespace cake