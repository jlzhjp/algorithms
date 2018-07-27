#ifndef __SELECTION_SORT_HPP__
#define __SELECTION_SORT_HPP__

#include <utility>
#include "../utility.hpp"

namespace alg {

template <typename ForwardIt>
inline void selection_sort(ForwardIt first, ForwardIt last) {
    selection_sort(first, last, compare_asc);
}

template <typename ForwardIt, typename TComparer>
void selection_sort(ForwardIt first, ForwardIt last, TComparer comp) {
    for (ForwardIt i = first; i != last; ++i) {
        ForwardIt k = i, j = i;
        for (++j; j != last; ++j) {
            if (comp(*j, *k) < 0) k = j;
        }
        std::swap(*i, *k);
    }
}

}  // namespace alg

#endif