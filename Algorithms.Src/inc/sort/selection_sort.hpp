#pragma once

#include <algorithm>
#include <iterator>
#include <utility>
#include "utility.hpp"

namespace alg {

template <typename ForwardIt, typename TComparer>
void selection_sort(ForwardIt first, ForwardIt last, TComparer comp) {
    for (; first != last; ++first) {
        std::swap(*first, *std::min_element(first, last, comp));
    }
}
template <typename ForwardIt>
inline void selection_sort(ForwardIt first, ForwardIt last) {
    selection_sort(
        first, last,
        compare_asc<typename std::iterator_traits<ForwardIt>::value_type>);
}

}  // namespace alg
