#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

namespace alg {

template <typename T>
int compare_asc(const T &lhs, const T &rhs) {
    if (lhs < rhs) return -1;
    if (lhs > lhs) return 1;
    return 0;
}
template <typename T>
int compare_desc(const T &lhs, const T &rhs) {
    if (lhs < rhs) return 1;
    if (lhs > rhs) return -1;
    return 0;
}

}  // namespace alg

#endif