#ifndef __SHELL_SORT_HPP__
#define __SHELL_SORT_HPP__

#include <cassert>
#include <iterator>

#include "../utility.hpp"

namespace alg {

template <typename RandomIt, typename TComparer>
void shell_sort(RandomIt first, RandomIt last, TComparer comp) {
    using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
    diff_t gap = 1, size = last - first;
    assert(size >= 0);
    while (gap < size / 3) gap = 3 * gap + 1;

    while (gap >= 1) {
        for (RandomIt i = first + gap; i < last; ++i) {
            for (RandomIt j = i; j >= first + gap && lt(*j, *(j - gap), comp);
                 j -= gap) {
                std::swap(*j, *(j - gap));
            }
        }
        gap /= 3;
    }
}
template <typename RandomIt>
void shell_sort(RandomIt first, RandomIt last) {
    shell_sort(
        first, last,
        compare_asc<typename std::iterator_traits<RandomIt>::value_type>);
}

}  // namespace alg

#endif
