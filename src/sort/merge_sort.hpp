#ifndef __MERGE_SORT_HPP__
#define __MERGE_SORT_HPP__

#include <memory>

#include "../array.hpp"
#include "../utility.hpp"

namespace alg {

template <typename T, size_t N, typename TComparer>
void __merge(Array<T, N> &arr, size_t lo, size_t mid, size_t hi,
             Array<T, N> &aux, TComparer comp) {
    size_t i = lo, j = mid + 1;

    std::uninitialized_move(arr.begin(), arr.end(), aux.begin());
    for (size_t k = lo; k <= hi; ++k) {
        if (i > mid)
            arr[k] = aux[j++];
        else if (j > hi)
            arr[k] = aux[i++];
        else if (lt(aux[j], aux[i], comp))
            arr[k] = aux[j++];
        else
            arr[k] = aux[i++];
    }
};
template <typename T, size_t N, typename TComparer>
void __merge_sort(Array<T, N> &arr, size_t lo, size_t hi, Array<T, N> &aux,
                  TComparer comp) {
    if (hi <= lo) return;
    size_t mid = lo + (hi - lo) / 2;
    __merge_sort(arr, lo, mid, aux, comp);      // 将左边排序
    __merge_sort(arr, mid + 1, hi, aux, comp);  // 将右边排序
    if (gt(arr[mid], arr[mid + 1], comp)) __merge(arr, lo, mid, hi, aux, comp);
}
template <typename T, size_t N, typename TComparer>
void merge_sort(Array<T, N> &arr, TComparer comp) {
    if constexpr (N <= 1) return;
    Array<T, N> aux;
    __merge_sort(arr, 0, N - 1, aux, comp);
}
template <typename T, size_t N>
void merge_sort(Array<T, N> &arr) {
    merge_sort(arr, compare_asc<T>);
}

}  // namespace alg

#endif
