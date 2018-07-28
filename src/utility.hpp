#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

namespace alg {

// Binary function that accepts two elements in the range as arguments,
// and returns a value convertible to bool.
// The value returned indicates whether the element passed as first argument is considered less than the second.
template <typename T>
bool compare_asc(const T &lhs, const T &rhs) {
    return lhs < rhs;
}
template <typename T>
bool compare_desc(const T &lhs, const T &rhs) {
    return lhs > rhs;
}

}  // namespace alg

#endif
