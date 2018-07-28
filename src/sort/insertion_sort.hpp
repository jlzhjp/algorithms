#ifndef __INSERTION_SORT__
#define __INSERTION_SORT__

#include <iterator>
#include <utility>

#include "../utility.hpp"

namespace alg {

template <typename BidirectIt, typename TComparer>
void insertion_sort(BidirectIt first, BidirectIt last, TComparer comp) {
    for (BidirectIt i = std::next(first); i != last; ++i) {
        for (BidirectIt j = i; j > first && comp(*j, *std::prev(j)); --j) {
            std::swap(*std::prev(j), *j);
        }
    }
}
template <typename BidirectIt>
inline void insertion_sort(BidirectIt first, BidirectIt last) {
    insertion_sort(
        first, last,
        compare_asc<typename std::iterator_traits<BidirectIt>::value_type>);
}

}  // namespace alg
#endif
