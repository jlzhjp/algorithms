#pragma once

#include <cassert>
#include <iostream>
#include "../vector.hpp"

namespace alg {
// 如果找到, 返回 [first, last) 之间的迭代器
// 如果未找到, 返回last
template <typename RandomIt, typename T>
RandomIt binary_search(RandomIt first, RandomIt last, const T &key) {
    RandomIt lo = first, hi = last - 1;
    while (lo <= hi) {
        RandomIt mid = lo + (hi - lo) / 2;
        if (key < *mid)
            hi = mid - 1;
        else if (key > *mid)
            lo = mid + 1;
        else
            return mid;
    }
    return last;
}

}  // namespace alg
