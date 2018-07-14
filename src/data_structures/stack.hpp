#ifndef __STACK_HPP__
#define __STACK_HPP__

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <type_traits>

namespace algo::ds {

template <typename T, typename TAlloc = std::allocator<T>>
class Stack {
public:
    using allocator_type = TAlloc;
    using value_type = typename std::allocator_traits<TAlloc>::value_type;
    using size_type = typename std::allocator_traits<TAlloc>::size_type;
    using reference = value_type &;
    using const_reference = const value_type &;

public:
    explicit Stack()
        : Stack(_kSpareSize) {}
    explicit Stack(size_type initcap)
        : _capacity(initcap),
          _data(allocate(initcap)) {}
    Stack(std::initializer_list<T> values)
        : _capacity(values.size() + _kSpareSize),
          _data(allocate(_capacity)) {
        for (auto value : values) push(value);
    }
    Stack(const Stack &rhs)
        : _size(rhs._size),
          _capacity(rhs._capacity),
          _data(allocate(rhs._capacity)),
          _alloc(rhs._alloc) {
        std::uninitialized_copy_n(rhs._data, rhs._size, _data);
    }
    Stack(Stack &&rhs)
        : _size(rhs._size),
          _capacity(rhs._capacity),
          _data(allocate(rhs._capacity)),
          _alloc(rhs._alloc) {
        std::uninitialized_move_n(rhs._data, rhs._size, _data);
    }
    ~Stack() {
        dispose();
    }

public:
    Stack& operator=(const Stack &rhs) {
        if (&rhs == this) return *this;
        clear();
        reserve(rhs._capacity);
        std::uninitialized_copy_n(rhs._data, rhs._size, _data);
        _size = rhs._size;
        _alloc = rhs._alloc;
        return *this;
    }
    Stack& operator=(Stack &&rhs) {
        if (&rhs == this) return *this;
        swap(rhs);
        rhs.dispose();
        return *this;
    }

public:
    reference top() { return _data[_size - 1]; }
    const_reference top() const { return _data[_size - 1]; }
    value_type pop() {
        value_type ret = std::move(_data[--_size]);
        if (_size < _capacity / 4) reserve(_capacity /= 2);
        return ret;
    }
    void push(const_reference val) {
        if (full()) reserve(_capacity *= 2);
        construct(_data + _size++, val);
    }
    void push(value_type &&val) {
        if (full()) reserve(_capacity *= 2);
        construct(_data + _size++, std::forward<value_type>(val));
    }
    void clear() {
        value_type *end = _data + _size;
        for (value_type *ptr = _data; ptr != end; ++ptr) {
            destroy(ptr);
        }
        _size = 0;
        reserve(_kSpareSize);
    }
    void swap(Stack &rhs) {
        std::swap(rhs._size, _size);
        std::swap(rhs._capacity, _capacity);
        std::swap(rhs._data, _data);
        std::swap(rhs._alloc, _alloc);
    }

public:
    bool empty() const noexcept { return _size == 0; }
    size_type size() const noexcept { return _size; }
    size_type capacity() const noexcept { return _capacity; }
    allocator_type get_allocator() const noexcept { return _alloc; }

private:
    static constexpr size_type _kSpareSize = 16;

private:
    value_type *allocate(size_type n) {
        value_type * p = std::allocator_traits<TAlloc>::allocate(_alloc, n);
        assert(p != nullptr);
        return p;
    }
    void deallocate(value_type *p, size_type n) noexcept {
        std::allocator_traits<TAlloc>::deallocate(_alloc, p, n);
    }
    template <typename... TArgs>
    void construct(value_type *p, TArgs... args) {
        std::allocator_traits<TAlloc>::construct(_alloc, p, std::forward<TArgs>(args)...);
    }
    void destroy(value_type *p) noexcept {
        std::allocator_traits<TAlloc>::destroy(_alloc, p);
    }
    void reserve(size_type n) {
        value_type *old = _data;
        _data = allocate(n);
        std::uninitialized_move_n(old, _size, _data);
        deallocate(old, _capacity);
        _capacity = n;
    }
    void dispose() {
        if (_disposed) return;
        value_type *end = _data + _size;
        for (value_type *ptr = _data; ptr != end; ++ptr) {
            destroy(ptr);
        }
        deallocate(_data, _capacity);
        _data = nullptr;
        _size = 0;
        _capacity = 0;
        _disposed = true;
    }

private:
    bool full() { return _size == _capacity; }

private:
    bool _disposed = false;
    size_type _size = 0;
    size_type _capacity = 0;
    value_type *_data = nullptr;
    allocator_type _alloc;
};

}

namespace std {

template <typename T>
void swap(ds::Stack<T> &lhs, ds::Stack<T>& rhs) {
    lhs.swap(rhs);
}

}

#endif
