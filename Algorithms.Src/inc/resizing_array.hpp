#pragma once

#include <cassert>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>

#include "memory.hpp"

namespace alg {

template <typename V, typename A = std::allocator<V>>
class ResizingArray {
private:
    using alloc_traits = std::allocator_traits<A>;

public:
    using allocator_type = A;
    using value_type = typename alloc_traits::value_type;
    using pointer = typename alloc_traits::pointer;
    using const_pointer = typename alloc_traits::const_pointer;
    using difference_type = typename alloc_traits::difference_type;
    using size_type = typename alloc_traits::size_type;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    explicit ResizingArray() : ResizingArray(SPARE_SPACE) {}
    explicit ResizingArray(size_type initcap) : _data(allocate(initcap)), _capacity(initcap) {}

    ResizingArray(std::initializer_list<value_type> vals)
        : ResizingArray(vals.size() + SPARE_SPACE) {
        for (auto &v : vals) {
            push_back(v);
        }
    }
    ResizingArray(const ResizingArray &rhs)
        : _data(allocate(rhs.capacity())),
          _capacity(rhs.capacity()),
          _size(rhs.size()),
          _alloc(rhs.get_allocator()) {
        uninit_copy_n_using_alloc(_alloc, rhs.data(), rhs.size(), _data);
    }
    ResizingArray(ResizingArray &&rhs) noexcept : ResizingArray(0) { swap(rhs); }

    ~ResizingArray() {
        clear();
        deallocate(this->_data, capacity());
    }

public:
    ResizingArray &operator=(const ResizingArray &rhs)
    {
        if (std::addressof(rhs) == this) return *this;
        ResizingArray tmp(rhs);
        swap(tmp);
        return *this;
    }
    ResizingArray &operator=(ResizingArray &&rhs) noexcept {
        if (std::addressof(rhs) == this) return *this;
        ResizingArray tmp(std::forward<ResizingArray>(rhs));
        swap(tmp);
        return *this;
    }
    reference operator[](size_type idx) {
        return const_cast<reference>(static_cast<const ResizingArray &>(*this)[idx]);
    }
    const_reference operator[](size_type idx) const {
        assert(0 <= idx && idx < _size);
        return _data[idx];
    }

public:
    reference at(size_type idx) {
        return const_cast<reference>(static_cast<const ResizingArray &>(*this).at(idx));
    }
    const_reference at(size_type idx) const {
        if (idx < 0 || idx >= _size) throw std::out_of_range("Index out of range.");
        return _data[idx];
    }
    void assign(size_type n, const_reference val) {
        clear();
        ensure_capacity_enough(n);
        uninit_fill_n_using_alloc(_alloc, _data, n, val);
        _size = n;
    }
    template <typename InputIter>
    void assign(InputIter first, InputIter last) {
        clear();
        auto dis = std::distance(first, last);
        ensure_capacity_enough(dis);
        uninit_copy_using_alloc(_alloc, first, last, _data);
        _size = dis;
    }
    void assign(std::initializer_list<value_type> vals) {
        clear();
        ensure_capacity_enough(vals.size());
        uninit_copy_n_using_alloc(_alloc, vals.begin(), vals.size(), _data);
        _size = vals.size();
    }
    void push_back(const_reference val) { emplace_back(val); }
    void push_back(value_type &&val) { emplace_back(std::forward<value_type>(val)); }
    void pop_back() {
        destroy(end() - 1);
        --_size;
    }
    template <typename... TArgs>
    void emplace_back(TArgs &&... args) {
        ensure_capacity_enough(_size + 1);
        construct(end(), std::forward<TArgs>(args)...);
        ++_size;
    }
    void clear() {
        destroy_n_using_alloc(_alloc, _data, _size);
        _size = 0;
    }
    void resize(size_type n) { resize(n, value_type()); }
    void resize(size_type n, const_reference val) {
        if (n > _size) {
            ensure_capacity_enough(n);
            uninit_fill_n_using_alloc(_alloc, end(), n, val);
            _size = n;
        } else if (n < _size) {
            destroy_using_alloc(_alloc, _data + n, _data + _size);
            _size = n;
        }
    }
    void reserve(size_type n) {
        if (n > _capacity) {
            change_capacity(n);
        }
    }
    void shrink_to_fit() { this->change_capacity(this->_size); }
    void swap(ResizingArray &rhs) noexcept {
        std::swap(_alloc, rhs._alloc);
        std::swap(_data, rhs._data);
        std::swap(_size, rhs._size);
        std::swap(_capacity, rhs._capacity);
    }

public:
    iterator begin() noexcept { return _data; }
    iterator end() noexcept { return _data + _size; }
    const_iterator begin() const noexcept { return _data; }
    const_iterator end() const noexcept { return _data + _size; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end() - 1); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin() - 1); }
    const_reverse_iterator rbegin() const noexcept { return rbegin(); }
    const_reverse_iterator rend() const noexcept { return rend(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }

    reference front() noexcept { return *begin(); }
    const_reference front() const noexcept { return *begin(); }
    reference back() noexcept { return *(end() - 1); }
    const_reference back() const noexcept { return *(end() - 1); }
    pointer data() noexcept { return _data; }
    const_pointer data() const noexcept { return _data; }

    bool empty() const noexcept { return _size == 0; }
    size_type size() const noexcept { return _size; }
    size_type capacity() const noexcept { return _capacity; }
    size_type max_size() const noexcept { return alloc_traits::max_size(_alloc); }
    allocator_type get_allocator() const noexcept { return _alloc; }

protected:
    allocator_type &alloc() noexcept { return _alloc; }
    pointer allocate(size_type n) { return alloc_traits::allocate(_alloc, n); }

    void deallocate(pointer p, size_type n) noexcept { alloc_traits::deallocate(_alloc, p, n); }

    template <typename... TArgs>
    void construct(pointer p, TArgs &&... args) {
        alloc_traits::construct(_alloc, p, std::forward<TArgs>(args)...);
    }
    void destroy(pointer p) noexcept { alloc_traits::destroy(_alloc, p); }

    void ensure_capacity_enough(size_type n) {
        if (n > _capacity) {
            change_capacity((n + SPARE_SPACE) * 2);
        }
    }

    void change_capacity(size_type n) {
        pointer newp = allocate(n);
        uninit_move_n_using_alloc(_alloc, _data, _size, newp);
        deallocate(_data, _capacity);
        _data = newp;
        _capacity = n;
    }
    void set_size(size_type n) noexcept { _size = n; }

private:
    static constexpr size_type SPARE_SPACE = 16;

private:
    pointer _data = nullptr;
    size_type _capacity = 0;
    size_type _size = 0;
    allocator_type _alloc;
};

template <typename V, typename A>
inline bool operator==(const ResizingArray<V, A> &lhs, const ResizingArray<V, A> &rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename V, typename A>
inline bool operator!=(const ResizingArray<V, A> &lhs, const ResizingArray<V, A> &rhs) {
    return !(lhs == rhs);
}
template <typename V, typename A>
inline bool operator<(const ResizingArray<V, A> &lhs, const ResizingArray<V, A> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template <typename V, typename A>
inline bool operator>(const ResizingArray<V, A> &lhs, const ResizingArray<V, A> &rhs) {
    return rhs < lhs;
}
template <typename V, typename A>
inline bool operator<=(const ResizingArray<V, A> &lhs, const ResizingArray<V, A> &rhs) {
    return !(lhs > rhs);
}
template <typename V, typename A>
inline bool operator>=(const ResizingArray<V, A> &lhs, const ResizingArray<V, A> &rhs) {
    return !(lhs < rhs);
}

template <typename V, typename A>
inline void swap(ResizingArray<V, A> &x, ResizingArray<V, A> &y) {
    x.swap(y);
}
}  // namespace alg
