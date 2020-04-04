#pragma once

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iterator>
#include <random>
#include "utility.hpp"

namespace alg {

template <typename RandomIt, typename TComparer>
RandomIt __partition(RandomIt lo, RandomIt hi, TComparer comp) {
    RandomIt i = lo, j = hi + 1;
    auto v = *lo;
    while (true) {
        while (lt(*(++i), v, comp))
            if (i == hi) break;
        while (ge(*(--j), v, comp))
            if (j == lo) break;
        if (i >= j) break;
        std::swap(*i, *j);
    }
    std::swap(*lo, *j);
    return j;
}

template <typename RandomIt, typename TComparer>
void __quick_sort(RandomIt lo, RandomIt hi, TComparer comp) {
    if (lo >= hi) return;
    RandomIt j = __partition(lo, hi, comp);
    __quick_sort(lo, j, comp);
    __quick_sort(std::next(j), hi, comp);
}

template <typename RandomIt, typename TComparer>
void quick_sort(RandomIt beg, RandomIt end, TComparer comp) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(beg, end, std::default_random_engine(seed));
    __quick_sort(beg, std::prev(end), comp);
}

template <typename RandomIt>
inline void quick_sort(RandomIt beg, RandomIt end) {
    quick_sort(
        beg, end,
        compare_asc<typename std::iterator_traits<RandomIt>::value_type>);
}
}  // namespace alg
