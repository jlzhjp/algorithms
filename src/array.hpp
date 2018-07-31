#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#include <cstddef>
#include <iterator>
#include <memory>

namespace alg {

template <typename T, size_t N>
struct Array {
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    reference operator[](size_type i) noexcept;
    const_reference operator[](size_type i) const noexcept;

    void fill(const value_type &val);
    void swap(Array &other);

    constexpr iterator begin() noexcept;
    constexpr iterator end() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr const_iterator end() const noexcept;
    constexpr const_iterator cbegin() const;
    constexpr const_iterator cend() const;
    constexpr reverse_iterator rbegin() noexcept;
    constexpr reverse_iterator rend() noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

    reference at(size_type i);
    const_reference at(size_type i) const;
    reference back() noexcept;
    const_reference back() const noexcept;
    reference front() noexcept;
    const_reference front() const noexcept;

    pointer data() noexcept;
    const_pointer data() const noexcept;
    size_type size() const noexcept;
    constexpr bool empty() const noexcept;
    constexpr size_type max_size() const noexcept;

    value_type _data[N == 0 ? 1 : N];
};

template <typename T, size_t N>
inline auto Array<T, N>::operator[](size_type i) noexcept -> reference {
    assert(i < size());
    return _data[i];
}
template <typename T, size_t N>
inline auto Array<T, N>::operator[](size_type i) const noexcept
    -> const_reference {
    assert(i < size());
    return _data[i];
}
template <typename T, size_t N>
inline void Array<T, N>::fill(const value_type &val) {
    std::uninitialized_fill(begin(), end(), val);
}
template <typename T, size_t N>
void Array<T, N>::swap(Array &other) {
    for (size_type i = 0; i != size(); ++i) {
        std::swap(_data[i], other[i]);
    }
}
template <typename T, size_t N>
constexpr auto Array<T, N>::begin() noexcept -> iterator {
    return _data;
}
template <typename T, size_t N>
constexpr auto Array<T, N>::end() noexcept -> iterator {
    return _data + N;
}
template <typename T, size_t N>
constexpr auto Array<T, N>::begin() const noexcept -> const_iterator {
    return _data;
}
template <typename T, size_t N>
constexpr auto Array<T, N>::end() const noexcept -> const_iterator {
    return _data + N;
}
template <typename T, size_t N>
constexpr auto Array<T, N>::cbegin() const -> const_iterator {
    return begin();
}
template <typename T, size_t N>
constexpr auto Array<T, N>::cend() const -> const_iterator {
    return end();
}
template <typename T, size_t N>
constexpr auto Array<T, N>::rbegin() noexcept -> reverse_iterator {
    return reverse_iterator(end());
}
template <typename T, size_t N>
constexpr auto Array<T, N>::rend() noexcept -> reverse_iterator {
    return reverse_iterator(begin());
}
template <typename T, size_t N>
constexpr auto Array<T, N>::rbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(end());
}
template <typename T, size_t N>
constexpr auto Array<T, N>::rend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(begin());
}
template <typename T, size_t N>
constexpr auto Array<T, N>::crbegin() const noexcept -> const_reverse_iterator {
    return rbegin();
}
template <typename T, size_t N>
constexpr auto Array<T, N>::crend() const noexcept -> const_reverse_iterator {
    return rend();
}
template <typename T, size_t N>
auto Array<T, N>::at(size_type i) -> reference {
    if (i < 0 && i >= size()) {
        throw std::out_of_range("Index out of range: " + std::to_string(i) +
                                ".");
    }
    return _data[i];
}
template <typename T, size_t N>
auto Array<T, N>::at(size_type i) const -> const_reference {
    if (i < 0 && i >= size()) {
        throw std::out_of_range("Index out of range: " + std::to_string(i) +
                                ".");
    }
    return _data[i];
}
template <typename T, size_t N>
inline auto Array<T, N>::front() noexcept -> reference {
    return _data[0];
}
template <typename T, size_t N>
inline auto Array<T, N>::front() const noexcept -> const_reference {
    return _data[0];
}
template <typename T, size_t N>
inline auto Array<T, N>::back() noexcept -> reference {
    return _data[size() - 1];
}
template <typename T, size_t N>
inline auto Array<T, N>::back() const noexcept -> const_reference {
    return _data[size() - 1];
}
template <typename T, size_t N>
inline auto Array<T, N>::data() noexcept -> pointer {
    return _data;
}
template <typename T, size_t N>
inline auto Array<T, N>::data() const noexcept -> const_pointer {
    return _data;
}
template <typename T, size_t N>
constexpr bool Array<T, N>::empty() const noexcept {
    return N == 0;
}
template <typename T, size_t N>
inline auto Array<T, N>::size() const noexcept -> size_type {
    return std::distance(begin(), end());
}
template <typename T, size_t N>
constexpr auto Array<T, N>::max_size() const noexcept -> size_type {
    return N;
}

template <typename T, size_t N>
inline bool operator==(const Array<T, N> &lhs, const Array<T, N> &rhs) {
    return lhs.size() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename T, size_t N>
inline bool operator!=(const Array<T, N> &lhs, const Array<T, N> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                        rhs.end());
}
template <typename T, size_t N>
inline bool operator<(const Array<T, N> &lhs, const Array<T, N> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                        rhs.end());
}
template <typename T, size_t N>
inline bool operator>(const Array<T, N> &lhs, const Array<T, N> &rhs) {
    return rhs < lhs;
}
template <typename T, size_t N>
inline bool operator<=(const Array<T, N> &lhs, const Array<T, N> &rhs) {
    return !(lhs > rhs);
}
template <typename T, size_t N>
inline bool operator>=(const Array<T, N> &lhs, const Array<T, N> &rhs) {
    return !(lhs < rhs);
}

}  // namespace alg

namespace std {

template <typename T, size_t N>
inline void swap(alg::Array<T, N> &x, alg::Array<T, N> &y) {
    x.swap(y);
}

}  // namespace std
#endif
