#include <tapes/sort.h>

namespace tapes {

template void multiway_merge_sort<std::less<>>::sort(itape&, itape&);
template void multiway_merge_sort<std::greater<>>::sort(itape&, itape&);

} // namespace tapes
