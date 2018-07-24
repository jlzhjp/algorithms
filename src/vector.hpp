#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <cassert>
#include <initializer_list>
#include <iterator>
#include <memory>

namespace alg::ds {

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
    using reference = T &;
    using const_reference = const T &;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

   public:
    Vector() : Vector(_kSpareSpace) {}
    Vector(size_type initcap) : _capacity(initcap), _data(allocate(initcap)) {}
    Vector(std::initializer_list<value_type> vals);
    Vector(const Vector &rhs);
    Vector(Vector &&rhs);
    ~Vector();

   public:
    Vector &operator=(const Vector &rhs);
    Vector &operator=(Vector &&rhs);
    reference operator[](size_type n) {
        assert(n >= 0 && n < size());
        return _data[n];
    }
    const_reference operator[](size_type n) const {
        assert(n >= 0 && n < size());
        return _data[n];
    }

   public:
    void pop_back() { erase(end() - 1); }
    void push_back(const value_type &val) { insert(end(), val); }
    void push_back(value_type &&val) {
        insert(end(), std::forward<value_type>(val));
    }
    template <typename... TArgs>
    iterator emplace(const_iterator pos, TArgs &&... args) {
        return insert(pos, value_type(std::forward<TArgs>(args)...));
    }
    template <typename... TArgs>
    void emplace_back(TArgs &&... args) {
        emplace(end(), std::forward<TArgs>(args)...);
    }
    iterator insert(const_iterator pos, const value_type &val);
    iterator insert(const_iterator pos, value_type &&val);
    iterator insert(const_iterator pos, size_type count, const value_type &val);
    iterator insert(const_iterator pos, std::initializer_list<value_type> vals);
    iterator erase(const_iterator pos) { return erase(pos, pos + 1); }
    iterator erase(const_iterator begin, const_iterator end);
    void clear();
    void reserve(size_type n);
    void shrink_to_fit() { reserve(_size); }
    void swap(Vector &rhs);

   public:
    iterator begin() noexcept { return _data; }
    iterator end() noexcept { return _data + _size; }
    const_iterator begin() const noexcept { return _data; }
    const_iterator end() const noexcept { return _data + _size; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }
    reference front() noexcept { return _data[0]; }
    const_reference front() const noexcept { return front(); }
    reference back() noexcept { return _data[_size - 1]; }
    const_reference back() const noexcept { return back(); }
    pointer data() noexcept { return _data; }
    const_pointer data() const noexcept { return data(); }
    bool empty() const noexcept { return _size == 0; }
    size_type size() const noexcept { return _size; }
    size_type capacity() const noexcept { return _capacity; }
    allocator_type get_allocator() const noexcept { return _alloc; }

   private:
    value_type *allocate(size_type n) {
        value_type *p = alloc_traits::allocate(_alloc, n);
        assert(p != nullptr);
        return p;
    }
    void deallocate(value_type *p, size_type n) noexcept {
        alloc_traits::deallocate(_alloc, p, n);
    }
    template <typename... TArgs>
    void construct(value_type *p, TArgs &&... args) {
        alloc_traits::construct(_alloc, p, std::forward<TArgs>(args)...);
    }
    void destroy(value_type *p) noexcept { alloc_traits::destroy(_alloc, p); }
    pointer alloc_block_n(const_iterator pos, size_type n);

   private:
    bool full() const noexcept { return _capacity == _size; }
    pointer end_ptr() noexcept { return _data + _size; }
    pointer begin_ptr() noexcept { return _data; }
    pointer end_ptr() const noexcept { return _data + _size; }
    pointer begin_ptr() const noexcept { return _data; }
    pointer get_ptr(const_iterator iter) noexcept {
        auto index = std::distance(cbegin(), iter);
        assert(index >= 0);
        return begin_ptr() + index;
    }
    const_pointer get_ptr(const_iterator iter) const noexcept {
        auto index = std::distance(cbegin(), iter);
        assert(index >= 0);
        return begin_ptr() + index;
    }

   private:
    static constexpr size_type _kSpareSpace = 16;

   private:
    bool _disposed = false;
    size_type _size = 0;
    size_type _capacity = 0;
    value_type *_data = nullptr;
    allocator_type _alloc;
};

template <typename T, typename TAlloc>
Vector<T, TAlloc>::Vector(std::initializer_list<value_type> vals)
    : Vector(vals.size() + _kSpareSpace) {
    for (auto &val : vals) {
        push_back(val);
    }
}
template <typename T, typename TAlloc>
Vector<T, TAlloc>::Vector(const Vector &rhs)
    : _size(rhs._size),
      _capacity(rhs._capacity),
      _data(allocate(rhs._capacity)),
      _alloc(rhs._alloc) {
    std::uninitialized_copy_n(rhs._data, rhs._size, _data);
}
template <typename T, typename TAlloc>
Vector<T, TAlloc>::Vector(Vector &&rhs) {
    swap(rhs);
}
template <typename T, typename TAlloc>
Vector<T, TAlloc>::~Vector() {
    assert(!_disposed);
    erase(begin(), end());
    deallocate(_data, _capacity);
    _data = nullptr;
    _capacity = 0;
}
template <typename T, typename TAlloc>
auto Vector<T, TAlloc>::operator=(const Vector &rhs) -> Vector & {
    if (std::addressof(rhs) == this) return *this;
    erase(begin(), end());
    reserve(rhs._capacity);
    std::uninitialized_copy_n(rhs.begin(), rhs.size(), begin());
    _size = rhs._size;
    _alloc = rhs._alloc;
    return *this;
}
template <typename T, typename TAlloc>
auto Vector<T, TAlloc>::operator=(Vector &&rhs) -> Vector & {
    if (std::addressof(rhs) == this) return *this;
    swap(rhs);
    rhs.~Vector();
    return *this;
}
template <typename T, typename TAlloc>
auto Vector<T, TAlloc>::insert(const_iterator pos, const value_type &val)
    -> iterator {
    pointer p = alloc_block_n(pos, 1);
    construct(p, val);
    ++_size;
    return iterator(p);
}
template <typename T, typename TAlloc>
auto Vector<T, TAlloc>::insert(const_iterator pos, value_type &&val)
    -> iterator {
    pointer p = alloc_block_n(pos, 1);
    construct(p, std::forward<value_type>(val));
    ++_size;
    return iterator(p);
}
template <typename T, typename TAlloc>
auto Vector<T, TAlloc>::insert(const_iterator pos, size_type count,
                               const value_type &val) -> iterator {
    pointer p = alloc_block_n(pos, count);
    std::uninitialized_fill_n(p, count, val);
    _size += count;
    return iterator(p);
}
template <typename T, typename TAlloc>
auto Vector<T, TAlloc>::insert(const_iterator pos,
                               std::initializer_list<value_type> vals)
    -> iterator {
    pointer p = alloc_block_n(pos, vals.size());
    std::uninitialized_copy(vals.begin(), vals.end(), iterator(p));
    _size += vals.size();
    return iterator(p);
}
template <typename T, typename TAlloc>
auto Vector<T, TAlloc>::erase(const_iterator b, const_iterator e) -> iterator {
    pointer pbegin = get_ptr(b);
    for (pointer p = pbegin; p != e; ++p) {
        destroy(p);
    }
    for (pointer pb = pbegin, pe = get_ptr(e); pe != end(); ++pb, ++pe) {
        construct(pb, std::move(*pe));
    }
    return iterator(pbegin);
}
template <typename T, typename TAlloc>
void Vector<T, TAlloc>::reserve(size_type n) {
    pointer old = _data;
    _data = allocate(n);
    std::uninitialized_move_n(old, _size, _data);
    deallocate(old, _capacity);
    _capacity = n;
}
template <typename T, typename TAlloc>
void Vector<T, TAlloc>::clear() {
    erase(begin(), end());
    reserve(_kSpareSpace);
}
template <typename T, typename TAlloc>
void Vector<T, TAlloc>::swap(Vector &rhs) {
    std::swap(rhs._size, _size);
    std::swap(rhs._capacity, _capacity);
    std::swap(rhs._data, _data);
    std::swap(rhs._alloc, _alloc);
}
template <typename T, typename TAlloc>
auto Vector<T, TAlloc>::alloc_block_n(const_iterator pos, size_type n)
    -> pointer {
    pointer ppos = get_ptr(pos);
    if (_capacity < size() + n) {
        auto index = std::distance(cbegin(), pos);
        assert(index >= 0);
        reserve(2 * (size() + n));
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
template <typename T, typename TAlloc>
inline bool operator==(const Vector<T, TAlloc> &lhs,
                       const Vector<T, TAlloc> &rhs) {
    return lhs.size() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename T, typename TAlloc>
inline bool operator!=(const Vector<T, TAlloc> &lhs,
                       const Vector<T, TAlloc> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                        rhs.end());
}
template <typename T, typename TAlloc>
inline bool operator<(const Vector<T, TAlloc> &lhs,
                      const Vector<T, TAlloc> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                        rhs.end());
}
template <typename T, typename TAlloc>
inline bool operator>(const Vector<T, TAlloc> &lhs,
                      const Vector<T, TAlloc> &rhs) {
    return rhs < lhs;
}
template <typename T, typename TAlloc>
inline bool operator<=(const Vector<T, TAlloc> &lhs,
                       const Vector<T, TAlloc> &rhs) {
    return !(lhs > rhs);
}
template <typename T, typename TAlloc>
inline bool operator>=(const Vector<T, TAlloc> &lhs,
                       const Vector<T, TAlloc> &rhs) {
    return !(lhs < rhs);
}
}  // namespace alg::ds

namespace std {
template <typename T>
void swap(alg::ds::Vector<T> &lhs, alg::ds::Vector<T> &rhs) {
    lhs.swap(rhs);
}
}  // namespace std

#endif
