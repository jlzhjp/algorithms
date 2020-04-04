#pragma once

#include <cassert>
#include <iterator>

#include "array.hpp"
#include "utility.hpp"

namespace alg {

template <typename T, size_t N, typename TComparer>
void shell_sort(Array<T, N>& arr, TComparer comp) {
    if constexpr (N < 2) return;
    size_t gap = 1;
    while (gap < arr.size() / 3) gap = 3 * gap + 1;
    while (gap >= 1) {
        for (size_t i = gap; i != arr.size(); ++i) {
            for (size_t j = i; j >= gap && lt(arr[j], arr[j - gap], comp);
                 j -= gap) {
                std::swap(arr[j], arr[j - gap]);
            }
        }
        gap /= 3;
    }
}
template <typename T, size_t N>
inline void shell_sort(Array<T, N>& arr) {
    shell_sort(arr, compare_asc<T>);
}

}  // namespace alg
