#ifndef __INSERTION_SORT__
#define __INSERTION_SORT__

#include <algorithm>
#include <iterator>
#include <utility>

#include "../utility.hpp"

namespace alg {

template <typename BidirectIt, typename TComparer>
void insertion_sort(BidirectIt first, BidirectIt last, TComparer comp) {
    // 找出最小的元素并放置于数组的左边, 去掉内循环的 j > first 条件
    std::swap(*first, *std::min_element(first, last, comp));
    for (BidirectIt i = std::next(first); i != last; ++i) {
        // 在内循环中将较大的元素向右移动而不总是交换两个元素
        auto tmp = std::move(*i);
        BidirectIt j = i;
        for (; comp(tmp, *std::prev(j)); --j) {
            *j = std::move(*std::prev(j));
        }
        *j = std::move(tmp);
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
