#ifndef __SELECTION_SORT_HPP__
#define __SELECTION_SORT_HPP__

#include <iterator>
#include <utility>
#include "../utility.hpp"

namespace alg {

template <typename ForwardIt, typename TComparer>
void selection_sort(ForwardIt first, ForwardIt last, TComparer comp) {
    for (ForwardIt i = first; i != last; ++i) {
        ForwardIt k = i;
        for (ForwardIt j = std::next(i); j != last; ++j) {
            if (comp(*j, *k) < 0) k = j;
        }
        std::swap(*i, *k);
    }
}
template <typename ForwardIt>
inline void selection_sort(ForwardIt first, ForwardIt last) {
    selection_sort(
        first, last,
        compare_asc<typename std::iterator_traits<ForwardIt>::value_type>);
}

}  // namespace alg

#endif
