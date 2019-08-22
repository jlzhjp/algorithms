#pragma once

namespace alg {

// Binary function that accepts two elements in the range as arguments,
// and returns a value convertible to bool.
// The value returned indicates whether the element passed as first argument is
// considered less than the second.
template <typename T>
inline bool compare_asc(const T &lhs, const T &rhs) {
    return lhs < rhs;
}
template <typename T>
inline bool compare_desc(const T &lhs, const T &rhs) {
    return lhs > rhs;
}
// ==
template <typename T, typename TComparer>
inline bool eq(const T &lhs, const T &rhs, TComparer comp) {
    return !comp(lhs, rhs) && !comp(rhs, lhs);
}
// !=
template <typename T, typename TComparer>
inline bool ne(const T &lhs, const T &rhs, TComparer comp) {
    return comp(lhs, rhs) || comp(rhs, lhs);
}
// <
template <typename T, typename TComparer>
inline bool lt(const T &lhs, const T &rhs, TComparer comp) {
    return comp(lhs, rhs);
}
// >
template <typename T, typename TComparer>
inline bool gt(const T &lhs, const T &rhs, TComparer comp) {
    return comp(rhs, lhs);
}
// <=
template <typename T, typename TComparer>
inline bool le(const T &lhs, const T &rhs, TComparer comp) {
    return !comp(rhs, lhs);
}
// >=
template <typename T, typename TComparer>
inline bool ge(const T &lhs, const T &rhs, TComparer comp) {
    return !comp(lhs, rhs);
}

// ==
template <typename T>
inline bool eq(const T &lhs, const T &rhs) {
    return eq(lhs, rhs, compare_asc<T>);
}
// !=
template <typename T>
inline bool ne(const T &lhs, const T &rhs) {
    return ne(lhs, rhs, compare_asc<T>);
}
// <
template <typename T>
inline bool lt(const T &lhs, const T &rhs) {
    return lt(lhs, rhs, compare_asc<T>);
}
// >
template <typename T>
inline bool gt(const T &lhs, const T &rhs) {
    return gt(lhs, rhs, compare_asc<T>);
}
// <=
template <typename T>
inline bool le(const T &lhs, const T &rhs) {
    return le(lhs, rhs, compare_asc<T>);
}
// >=
template <typename T>
inline bool ge(const T &lhs, const T &rhs) {
    return ge(lhs, rhs, compare_asc<T>);
}

}  // namespace alg
