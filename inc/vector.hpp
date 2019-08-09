#pragma once

#include <cassert>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>

namespace alg {

template <typename T, typename TAlloc = std::allocator<T>>
class Vector {
private:
    using alloc_traits = std::allocator_traits<TAlloc>;

public:
    using allocator_type = TAlloc;
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
    explicit Vector();
    explicit Vector(size_type initcap);
    Vector(std::initializer_list<value_type> vals);
    Vector(const Vector &rhs);
    Vector(Vector &&rhs);
    ~Vector();

public:
    Vector &operator=(const Vector &rhs);
    Vector &operator=(Vector &&rhs);
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;

public:
    void assign(size_type count, const value_type &val);
    template <typename InputIter>
    void assign(InputIter first, InputIter last);
    void assign(std::initializer_list<value_type> vals);
    void push_back(const value_type &val);
    void push_back(value_type &&val);
    void pop_back();
    template <typename... TArgs>
    iterator emplace(const_iterator pos, TArgs &&... args);
    template <typename... TArgs>
    void emplace_back(TArgs &&... args);
    iterator insert(const_iterator pos, const value_type &val);
    iterator insert(const_iterator pos, value_type &&val);
    iterator insert(const_iterator pos, size_type count, const value_type &val);
    iterator insert(const_iterator pos, std::initializer_list<value_type> vals);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator begin, const_iterator end);
    void clear();
    void resize(size_type n);
    void resize(size_type n, const value_type &val);
    void reserve(size_type n);
    void shrink_to_fit();
    void swap(Vector &rhs);

public:
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const;
    const_iterator cend() const;
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    reference at(size_type i);
    const_reference at(size_type i) const;
    reference front() noexcept;
    const_reference front() const noexcept;
    reference back() noexcept;
    const_reference back() const noexcept;
    pointer data() noexcept;
    const_pointer data() const noexcept;

    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type capacity() const noexcept;
    size_type max_size() const noexcept;
    allocator_type get_allocator() const noexcept;

private:
    bool full() const noexcept;
    pointer begin_ptr() noexcept;
    pointer end_ptr() noexcept;
    pointer begin_ptr() const noexcept;
    pointer end_ptr() const noexcept;

    template <typename... TArgs>
    void construct(value_type *p, TArgs &&... args);
    value_type *allocate(size_type n);
    void deallocate(value_type *p, size_type n) noexcept;
    void destroy(value_type *p) noexcept;

    pointer get_ptr(const_iterator iter) noexcept;
    const_pointer get_ptr(const_iterator iter) const noexcept;
    pointer alloc_block_n(const_iterator pos, size_type n);
    void modify_capacity(size_type n);

private:
    static constexpr size_type _kSpareSpace = 16;

private:
    bool _disposed = false;
    size_type _size = 0;
    size_type _capacity = 0;
    value_type *_data = nullptr;
    allocator_type _alloc;
};

template <typename V, typename A>
Vector<V, A>::Vector() : Vector(_kSpareSpace) {}
template <typename V, typename A>
Vector<V, A>::Vector(size_type initcap)
    : _capacity(initcap), _data(allocate(initcap)) {}
template <typename V, typename A>
Vector<V, A>::Vector(std::initializer_list<value_type> vals)
    : Vector(vals.size() + _kSpareSpace) {
    for (auto &val : vals) {
        push_back(val);
    }
}
template <typename V, typename A>
Vector<V, A>::Vector(const Vector &rhs)
    : _size(rhs._size),
      _capacity(rhs._capacity),
      _data(allocate(rhs._capacity)),
      _alloc(rhs._alloc) {
    std::uninitialized_copy_n(rhs._data, rhs._size, _data);
}
template <typename V, typename A>
inline Vector<V, A>::Vector(Vector &&rhs) {
    *this = std::forward<Vector>(rhs);
}
template <typename V, typename A>
Vector<V, A>::~Vector() {
    assert(!_disposed);
    erase(begin(), end());
    deallocate(_data, _capacity);
    _data = nullptr;
    _capacity = 0;
}
template <typename V, typename A>
auto Vector<V, A>::operator=(const Vector &rhs) -> Vector & {
    if (std::addressof(rhs) == this) return *this;
    erase(begin(), end());
    modify_capacity(rhs._capacity);
    std::uninitialized_copy_n(rhs.begin(), rhs.size(), begin());
    _size = rhs._size;
    _alloc = rhs._alloc;
    return *this;
}
template <typename V, typename A>
auto Vector<V, A>::operator=(Vector &&rhs) -> Vector & {
    if (std::addressof(rhs) == this) return *this;
    erase(begin(), end());
    delete _data;
    _data = rhs._data;
    _alloc = rhs._alloc;
    _size = rhs._size;
    _capacity = rhs._capacity;
    rhs._data = nullptr;
    rhs._size = 0;
    rhs._capacity = 0;
    return *this;
}
template <typename V, typename A>
inline auto Vector<V, A>::operator[](size_type n) -> reference {
    assert(n >= 0 && n < size());
    return _data[n];
}
template <typename V, typename A>
inline auto Vector<V, A>::operator[](size_type n) const -> const_reference {
    assert(n >= 0 && n < size());
    return _data[n];
}
template <typename V, typename A>
void Vector<V, A>::assign(size_type count, const value_type &val) {
    clear();
    if (capacity() < count) modify_capacity(count + _kSpareSpace);
    std::uninitialized_fill_n(begin(), count, val);
    _size = count;
}
template <typename V, typename A>
template <typename InputIter>
void Vector<V, A>::assign(InputIter first, InputIter last) {
    clear();
    for (InputIter iter = first; iter != last; ++iter) push_back(*iter);
}
template <typename V, typename A>
void Vector<V, A>::assign(std::initializer_list<value_type> vals) {
    clear();
    if (capacity() < vals.size()) modify_capacity(vals.size() + _kSpareSpace);
    std::uninitialized_copy(vals.begin(), vals.end(), begin());
    _size = vals.size();
}
template <typename V, typename A>
inline void Vector<V, A>::push_back(const value_type &val) {
    insert(end(), val);
}
template <typename V, typename A>
inline void Vector<V, A>::push_back(value_type &&val) {
    insert(end(), std::forward<value_type>(val));
}
template <typename V, typename A>
inline void Vector<V, A>::pop_back() {
    erase(end() - 1);
}
template <typename V, typename A>
template <typename... TArgs>
inline auto Vector<V, A>::emplace(const_iterator pos, TArgs &&... args)
    -> iterator {
    pointer p = alloc_block_n(pos, 1);
    construct(p, std::forward<TArgs>(args)...);
    ++_size;
    return iterator(p);
}
template <typename V, typename A>
template <typename... TArgs>
void Vector<V, A>::emplace_back(TArgs &&... args) {
    emplace(end(), std::forward<TArgs>(args)...);
}
template <typename V, typename A>
inline auto Vector<V, A>::insert(const_iterator pos, const value_type &val)
    -> iterator {
    return emplace(pos, val);
}
template <typename V, typename A>
inline auto Vector<V, A>::insert(const_iterator pos, value_type &&val)
    -> iterator {
    return emplace(pos, std::forward<value_type>(val));
}
template <typename V, typename A>
auto Vector<V, A>::insert(const_iterator pos, size_type count,
                          const value_type &val) -> iterator {
    pointer p = alloc_block_n(pos, count);
    std::uninitialized_fill_n(p, count, val);
    _size += count;
    return iterator(p);
}
template <typename V, typename A>
auto Vector<V, A>::insert(const_iterator pos,
                          std::initializer_list<value_type> vals) -> iterator {
    pointer p = alloc_block_n(pos, vals.size());
    std::uninitialized_copy(vals.begin(), vals.end(), iterator(p));
    _size += vals.size();
    return iterator(p);
}
template <typename V, typename A>
inline auto Vector<V, A>::erase(const_iterator pos) -> iterator {
    return erase(pos, pos + 1);
}
template <typename V, typename A>
auto Vector<V, A>::erase(const_iterator b, const_iterator e) -> iterator {
    pointer pbegin = get_ptr(b);
    if (b == e) return pbegin;
    size_type s = _size;
    for (pointer p = pbegin; p != e; ++p) {
        destroy(p);
        --s;
    }
    for (pointer pb = pbegin, pe = get_ptr(e); pe != end_ptr(); ++pb, ++pe) {
        construct(pb, std::move(*pe));
    }
    _size = s;
    return iterator(pbegin);
}
template <typename V, typename A>
inline void Vector<V, A>::clear() {
    erase(begin(), end());
}
template <typename V, typename A>
inline void Vector<V, A>::resize(size_type n) {
    resize(n, value_type());
}
template <typename V, typename A>
void Vector<V, A>::resize(size_type n, const value_type &val) {
    if (n < _size) {
        erase(begin() + n, end());
    } else if (n > _size) {
        insert(end(), n - size(), val);
    }
}
template <typename V, typename A>
void Vector<V, A>::reserve(size_type n) {
    if (n > capacity()) modify_capacity(n);
}
template <typename V, typename A>
inline void Vector<V, A>::shrink_to_fit() {
    modify_capacity(_size);
}
template <typename V, typename A>
void Vector<V, A>::swap(Vector &rhs) {
    std::swap(rhs._size, _size);
    std::swap(rhs._capacity, _capacity);
    std::swap(rhs._data, _data);
    std::swap(rhs._alloc, _alloc);
}
template <typename V, typename A>
inline auto Vector<V, A>::begin() noexcept -> iterator {
    return _data;
}
template <typename V, typename A>
inline auto Vector<V, A>::end() noexcept -> iterator {
    return _data + _size;
}
template <typename V, typename A>
inline auto Vector<V, A>::begin() const noexcept -> const_iterator {
    return _data;
}
template <typename V, typename A>
inline auto Vector<V, A>::end() const noexcept -> const_iterator {
    return _data + _size;
}
template <typename V, typename A>
inline auto Vector<V, A>::cbegin() const -> const_iterator {
    return begin();
}
template <typename V, typename A>
inline auto Vector<V, A>::cend() const -> const_iterator {
    return end();
}
template <typename V, typename A>
inline auto Vector<V, A>::rbegin() noexcept -> reverse_iterator {
    return reverse_iterator(end());
}
template <typename V, typename A>
inline auto Vector<V, A>::rend() noexcept -> reverse_iterator {
    return reverse_iterator(begin());
}
template <typename V, typename A>
inline auto Vector<V, A>::rbegin() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(end());
}
template <typename V, typename A>
inline auto Vector<V, A>::rend() const noexcept -> const_reverse_iterator {
    return const_reverse_iterator(begin());
}
template <typename V, typename A>
inline auto Vector<V, A>::crbegin() const noexcept -> const_reverse_iterator {
    return rbegin();
}
template <typename V, typename A>
inline auto Vector<V, A>::crend() const noexcept -> const_reverse_iterator {
    return rend();
}
template <typename V, typename A>
auto Vector<V, A>::at(size_type i) -> reference {
    if (i < 0 && i >= _size) {
        throw std::out_of_range("Index out of range: " + std::to_string(i) +
                                ".");
    }
    return _data[i];
}
template <typename V, typename A>
auto Vector<V, A>::at(size_type i) const -> const_reference {
    if (i < 0 && i >= _size) {
        throw std::out_of_range("Index out of range: " + std::to_string(i) +
                                ".");
    }
    return _data[i];
}
template <typename V, typename A>
inline auto Vector<V, A>::front() noexcept -> reference {
    return _data[0];
}
template <typename V, typename A>
inline auto Vector<V, A>::front() const noexcept -> const_reference {
    return _data[0];
}
template <typename V, typename A>
inline auto Vector<V, A>::back() noexcept -> reference {
    return _data[_size - 1];
}
template <typename V, typename A>
inline auto Vector<V, A>::back() const noexcept -> const_reference {
    return _data[_size - 1];
}
template <typename V, typename A>
inline auto Vector<V, A>::data() noexcept -> pointer {
    return _data;
}
template <typename V, typename A>
inline auto Vector<V, A>::data() const noexcept -> const_pointer {
    return _data;
}
template <typename V, typename A>
inline bool Vector<V, A>::empty() const noexcept {
    return _size == 0;
}
template <typename V, typename A>
inline auto Vector<V, A>::size() const noexcept -> size_type {
    return _size;
}
template <typename V, typename A>
inline auto Vector<V, A>::capacity() const noexcept -> size_type {
    return _capacity;
}
template <typename V, typename A>
inline auto Vector<V, A>::max_size() const noexcept -> size_type {
    return std::min(alloc_traits::max_size(_alloc),
                    std::numeric_limits<size_type>::max());
}
template <typename V, typename A>
inline auto Vector<V, A>::get_allocator() const noexcept -> allocator_type {
    return _alloc;
}
template <typename V, typename A>
bool Vector<V, A>::full() const noexcept {
    return _capacity == _size;
}
template <typename V, typename A>
inline auto Vector<V, A>::begin_ptr() noexcept -> pointer {
    return _data;
}
template <typename V, typename A>
inline auto Vector<V, A>::end_ptr() noexcept -> pointer {
    return _data + _size;
}
template <typename V, typename A>
inline auto Vector<V, A>::begin_ptr() const noexcept -> pointer {
    return _data;
}
template <typename V, typename A>
inline auto Vector<V, A>::end_ptr() const noexcept -> pointer {
    return _data + _size;
}
template <typename V, typename A>
inline auto Vector<V, A>::allocate(size_type n) -> value_type * {
    value_type *p = alloc_traits::allocate(_alloc, n);
    assert(p != nullptr);
    return p;
}
template <typename V, typename A>
inline void Vector<V, A>::deallocate(value_type *p, size_type n) noexcept {
    alloc_traits::deallocate(_alloc, p, n);
}
template <typename V, typename A>
template <typename... TArgs>
inline void Vector<V, A>::construct(value_type *p, TArgs &&... args) {
    alloc_traits::construct(_alloc, p, std::forward<TArgs>(args)...);
}
template <typename V, typename A>
inline void Vector<V, A>::destroy(value_type *p) noexcept {
    alloc_traits::destroy(_alloc, p);
}
template <typename V, typename A>
auto Vector<V, A>::get_ptr(const_iterator iter) noexcept -> pointer {
    auto index = std::distance(cbegin(), iter);
    assert(index >= 0);
    return begin_ptr() + index;
}
template <typename V, typename A>
auto Vector<V, A>::get_ptr(const_iterator iter) const noexcept
    -> const_pointer {
    auto index = std::distance(cbegin(), iter);
    assert(index >= 0);
    return begin_ptr() + index;
}
template <typename V, typename A>
auto Vector<V, A>::alloc_block_n(const_iterator pos, size_type n) -> pointer {
    pointer ppos = get_ptr(pos);
    if (_capacity < _size + n) {
        auto index = std::distance(cbegin(), pos);
        assert(index >= 0);
        modify_capacity(2 * (size() + n));
        ppos = begin_ptr() + index;
    }
    if (ppos == end_ptr()) return ppos;
    pointer p = end_ptr();
    do {
        --p;
        construct(p + n, std::move(*p));
    } while (p != ppos);
    return ppos;
}
template <typename V, typename A>
void Vector<V, A>::modify_capacity(size_type n) {
    assert(n >= _size);
    assert(n <= max_size());
    pointer old = _data;
    _data = allocate(n);
    std::uninitialized_move_n(old, _size, _data);
    deallocate(old, _capacity);
    _capacity = n;
}

template <typename V, typename A>
inline bool operator==(const Vector<V, A> &lhs, const Vector<V, A> &rhs) {
    return lhs.size() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename V, typename A>
inline bool operator!=(const Vector<V, A> &lhs, const Vector<V, A> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                        rhs.end());
}
template <typename V, typename A>
inline bool operator<(const Vector<V, A> &lhs, const Vector<V, A> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                        rhs.end());
}
template <typename V, typename A>
inline bool operator>(const Vector<V, A> &lhs, const Vector<V, A> &rhs) {
    return rhs < lhs;
}
template <typename V, typename A>
inline bool operator<=(const Vector<V, A> &lhs, const Vector<V, A> &rhs) {
    return !(lhs > rhs);
}
template <typename V, typename A>
inline bool operator>=(const Vector<V, A> &lhs, const Vector<V, A> &rhs) {
    return !(lhs < rhs);
}
}  // namespace alg

namespace std {
template <typename T>
inline void swap(alg::Vector<T> &lhs, alg::Vector<T> &rhs) {
    lhs.swap(rhs);
}
}  // namespace std
