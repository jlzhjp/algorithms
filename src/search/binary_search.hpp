#ifndef __BINARY_SEARCH_HPP__
#define __BINARY_SEARCH_HPP__

#include <cassert>
#include <iostream>
#include <vector>

namespace algo {

template <typename Comparable>
int binary_search(Comparable key, const std::vector<Comparable> &vec) {
    int lo = 0,
        hi = vec.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (key < vec[mid])
            hi = mid - 1;
        else if (key > vec[mid])
            lo = mid + 1;
        else
            return mid;
    }
    return -1;
}

} // namespace algo
#endif
