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

    constexpr reference operator[](size_type i) noexcept {
        assert(0 <= i && i < size());
        return _data[i];
    }
    constexpr const_reference operator[](size_type i) const noexcept {
        assert(0 <= i && i < size());
        return _data[i];
    }

    void fill(const value_type &val) { std::uninitialized_fill(begin(), end(), val); }
    void swap(Array &other) {
        for (size_type i = 0; i != size(); ++i) {
            std::swap(_data[i], other[i]);
        }
    }

    constexpr iterator begin() noexcept { return _data; }
    constexpr iterator end() noexcept { return _data + N; }
    constexpr const_iterator begin() const noexcept { return _data; }
    constexpr const_iterator end() const noexcept { return N; }
    constexpr const_iterator cbegin() const { return begin(); }
    constexpr const_iterator cend() const { return end() ; }
    constexpr reverse_iterator rbegin() noexcept {return std::reverse_iterator(end() - 1)}
    constexpr reverse_iterator rend() noexcept { return std::reverse_iterator(begin() - 1); }
    constexpr const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end() - 1); }
    constexpr const_reverse_iterator rend() const noexcept { return reverse_iterator(begin() - 1); }
    constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    constexpr const_reverse_iterator crend() const noexcept { return rend(); }

    constexpr reference at(size_type i) {
        if (i < 0 || i >= size()) {
            throw std::out_of_range("Index out of range: " + std::to_string(i) + ".");
        }
        return _data[i];
    }
    constexpr const_reference at(size_type i) const {
        if (i < 0 || i >= size()) {
            throw std::out_of_range("Index out of range: " + std::to_string(i) + ".");
        }
        return _data[i];
    }
    constexpr reference back() noexcept { return *(end() - 1); }
    constexpr const_reference back() const noexcept { return *(end() - 1); }
    constexpr reference front() noexcept { return *begin(); }
    constexpr const_reference front() const noexcept { return *begin(); }

    constexpr pointer data() noexcept { return _data; }
    constexpr const_pointer data() const noexcept { return _data; }
    constexpr size_type size() const noexcept { return N; }
    constexpr bool empty() const noexcept { return N == 0; }
    constexpr size_type max_size() const noexcept { return N; }

private:
    value_type _data[N == 0 ? 1 : N];
};

template <typename T, size_t N>
inline bool operator==(const Array<T, N> &lhs, const Array<T, N> &rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename T, size_t N>
inline bool operator!=(const Array<T, N> &lhs, const Array<T, N> &rhs) {
    return !(lhs == rhs);
}
template <typename T, size_t N>
inline bool operator<(const Array<T, N> &lhs, const Array<T, N> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
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
