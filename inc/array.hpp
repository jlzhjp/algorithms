#pragma once

#include <cassert>
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

    constexpr reference operator[](size_type i) noexcept;
    constexpr const_reference operator[](size_type i) const noexcept;

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

    constexpr reference at(size_type i);
    constexpr const_reference at(size_type i) const;
    constexpr reference back() noexcept;
    constexpr const_reference back() const noexcept;
    constexpr reference front() noexcept;
    constexpr const_reference front() const noexcept;

    constexpr pointer data() noexcept;
    constexpr const_pointer data() const noexcept;
    constexpr size_type size() const noexcept;
    constexpr bool empty() const noexcept;
    constexpr size_type max_size() const noexcept;

private:
    value_type _data[N == 0 ? 1 : N];
};

#define IMPL(declaration...)        \
    template <typename T, size_t N> \
    declaration Array<T, N>

IMPL(constexpr auto)::operator[](size_type i) noexcept -> reference {
    assert(0 <= i && i < size());
    return _data[i];
}
IMPL(constexpr auto)::operator[](size_type i) const noexcept -> const_reference {
    assert(0 <= i && i < size());
    return _data[i];
}
IMPL(inline void)::fill(const value_type &val) {
    std::uninitialized_fill(begin(), end(), val);
}
IMPL(void)::swap(Array &other) {
    for (size_type i = 0; i != size(); ++i) {
        std::swap(_data[i], other[i]);
    }
}
IMPL(constexpr auto)::begin() noexcept -> iterator {
    return _data;
}
IMPL(constexpr auto)::end() noexcept -> iterator {
    return _data + N;
}
IMPL(constexpr auto)::begin() const noexcept -> const_iterator {
    return _data;
}
IMPL(constexpr auto)::end() const noexcept -> const_iterator {
    return _data + N;
}
IMPL(constexpr auto)::cbegin() const -> const_iterator {
    return begin();
}
IMPL(constexpr auto)::cend() const -> const_iterator {
    return end();
}
IMPL(constexpr auto)::rbegin() noexcept -> reverse_iterator {
    return reverse_iterator(end());
}
IMPL(constexpr auto)::rend() noexcept -> reverse_iterator {
    return reverse_iterator(begin());
}
IMPL(constexpr auto)::rbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(end());
}
IMPL(constexpr auto)::rend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(begin());
}
IMPL(constexpr auto)::crbegin() const noexcept -> const_reverse_iterator {
    return rbegin();
}
IMPL(constexpr auto)::crend() const noexcept -> const_reverse_iterator {
    return rend();
}
IMPL(constexpr auto)::at(size_type i) -> reference {
    if (i < 0 || i >= size()) {
        throw std::out_of_range("Index out of range: " + std::to_string(i) +
                                ".");
    }
    return _data[i];
}
IMPL(constexpr auto)::at(size_type i) const -> const_reference {
    if (i < 0 || i >= size()) {
        throw std::out_of_range("Index out of range: " + std::to_string(i) +
                                ".");
    }
    return _data[i];
}
IMPL(constexpr auto)::front() noexcept -> reference {
    return _data[0];
}
IMPL(constexpr auto)::front() const noexcept -> const_reference {
    return _data[0];
}
IMPL(constexpr auto)::back() noexcept -> reference {
    return _data[size() - 1];
}
IMPL(constexpr auto)::back() const noexcept -> const_reference {
    return _data[size() - 1];
}
IMPL(constexpr auto)::data() noexcept -> pointer {
    return _data;
}
IMPL(constexpr auto)::data() const noexcept -> const_pointer {
    return _data;
}
IMPL(constexpr bool)::empty() const noexcept {
    return N == 0;
}
IMPL(constexpr auto)::size() const noexcept -> size_type {
    return std::distance(begin(), end());
}
IMPL(constexpr auto)::max_size() const noexcept -> size_type {
    return N;
}
template <typename T, size_t N>
inline bool operator==(const Array<T, N> &lhs, const Array<T, N> &rhs) {
    return lhs.size() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename T, size_t N>
inline bool operator!=(const Array<T, N> &lhs, const Array<T, N> &rhs) {
    return !(lhs == rhs);
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

template <typename T, size_t N>
inline void swap(alg::Array<T, N> &x, alg::Array<T, N> &y) {
    x.swap(y);
}
}  // namespace alg
