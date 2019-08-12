#pragma once

#include <cassert>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>

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
    explicit ResizingArray();
    explicit ResizingArray(size_type initcap);
    ResizingArray(std::initializer_list<value_type> vals);
    ResizingArray(const ResizingArray &rhs);
    ResizingArray(ResizingArray &&rhs);
    ~ResizingArray();

public:
    ResizingArray &operator=(const ResizingArray &rhs);
    ResizingArray &operator=(ResizingArray &&rhs);
    reference operator[](size_type idx);
    const_reference operator[](size_type idx) const;

public:
    reference at(size_type i);
    const_reference at(size_type i) const;
    void assign(size_type n, const_reference);
    template <typename InputIter>
    void assign(InputIter first, InputIter last);
    void assign(std::initializer_list<value_type> vals);
    void push_back(const_reference val);
    void push_back(value_type &&val);
    void pop_back();
    template <typename... TArgs>
    void emplace_back(TArgs &&... args);
    void clear();
    void resize(size_type n);
    void resize(size_type n, const_reference val);
    void reserve(size_type n);
    void shrink_to_fit();
    void swap(ResizingArray &rhs);

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

protected:
    void ensure_capacity_enough(size_type addition);
    void change_capacity(size_type n);

private:
    pointer allocate(size_type n);
    void deallocate(pointer p, size_type n) noexcept;
    template <typename... TArgs>
    void construct(pointer p, TArgs &&... args);
    void destroy(pointer p) noexcept;

private:
    static constexpr size_type SPARE_SPACE = 16;

private:
    pointer _data = nullptr;
    size_type _capacity = 0;
    size_type _size = 0;
    allocator_type _alloc;
};

// -----------------------------------------------------------------------------

#define IMPL(declaration...)          \
    template <typename V, typename A> \
    declaration ResizingArray<V, A>

IMPL(inline auto)::allocate(size_type n) -> pointer {
    return alloc_traits::allocate(_alloc, n);
}
IMPL(inline void)::deallocate(pointer p, size_type n) noexcept {
    alloc_traits::deallocate(_alloc, p, n);
}
IMPL(template <typename... TArgs> void)::construct(pointer p,
                                                   TArgs &&... args) {
    alloc_traits::construct(_alloc, p, std::forward<TArgs>(args)...);
}
IMPL(inline void)::destroy(pointer p) noexcept {
    alloc_traits::destroy(_alloc, p);
}
IMPL(void)::change_capacity(size_type n) {
    pointer newp = allocate(n);
    std::uninitialized_move_n(_data, _size, newp);
    deallocate(_data, _capacity);
    _data = newp;
    _capacity = n;
}
IMPL(void)::reserve(size_type n) {
    if (n > _capacity) {
        change_capacity(n);
    }
}
IMPL(void)::shrink_to_fit() { change_capacity(_size); }
IMPL(void)::ensure_capacity_enough(size_type addition) {
    if (_size + addition > _capacity) {
        reserve((_size + addition + SPARE_SPACE) * 2);
    }
}
IMPL()::ResizingArray(size_type initcap)
    : _data(allocate(initcap)), _capacity(initcap) {}
IMPL()::ResizingArray() : ResizingArray(SPARE_SPACE) {}
IMPL()::ResizingArray(std::initializer_list<value_type> vals)
    : ResizingArray(vals.size() + SPARE_SPACE) {
    for (auto &v : vals){
        push_back(v);
    }
}
IMPL()::ResizingArray(const ResizingArray &rhs)
    : _data(allocate(rhs.capacity())),
      _capacity(rhs.capacity()),
      _size(rhs.size()),
      _alloc(rhs.get_allocator()) {
    std::uninitialized_copy_n(rhs.data(), rhs.size(), _data);
}
IMPL()::ResizingArray(ResizingArray &&rhs) : ResizingArray(0) { swap(rhs); }
IMPL()::~ResizingArray() {
    clear();
    deallocate(_data, capacity());
}
IMPL(void)::swap(ResizingArray &rhs) {
    std::swap(_alloc, rhs._alloc);
    std::swap(_data, rhs._data);
    std::swap(_size, rhs._size);
    std::swap(_capacity, rhs._capacity);
}
IMPL(auto)::operator=(const ResizingArray &rhs) -> ResizingArray & {
    if (std::addressof(rhs) == this) return *this;
    ResizingArray tmp(rhs);
    swap(tmp);
    return *this;
}
IMPL(auto)::operator=(ResizingArray &&rhs) -> ResizingArray & {
    if (std::addressof(rhs) == this) return *this;
    ResizingArray tmp(std::forward<ResizingArray>(rhs));
    swap(tmp);
    return *this;
}
IMPL(auto)::operator[](size_type idx) -> reference {
    return const_cast<reference>(
        static_cast<const ResizingArray &>(*this)[idx]);
}
IMPL(auto)::operator[](size_type idx) const -> const_reference {
    assert(0 <= idx && idx < _size);
    return _data[idx];
}
IMPL(template <typename... TArgs> inline void)::emplace_back(TArgs &&... args) {
    ensure_capacity_enough(1);
    construct(end(), std::forward<TArgs>(args)...);
    ++_size;
}
IMPL(inline void)::push_back(const_reference val) { emplace_back(val); }
IMPL(inline void)::push_back(value_type &&val) {
    emplace_back(std::forward<value_type>(val));
}
IMPL(void)::pop_back() {
    destroy(end() - 1);
    --_size;
}
IMPL(void)::clear() {
    std::destroy(begin(), end());
    _size = 0;
}
IMPL(void)::assign(size_type n, const_reference val) {
    clear();
    ensure_capacity_enough(n);
    std::uninitialized_fill_n(_data, n, val);
    _size = n;
}
IMPL(template <typename InputIter> void)::assign(InputIter first,
                                                 InputIter last) {
    clear();
    ensure_capacity_enough(std::distance(first, last));
    std::uninitialized_copy(first, last, _data);
    _size = std::distance(first, last);
}
IMPL(void)::assign(std::initializer_list<value_type> vals) {
    clear();
    ensure_capacity_enough(vals.size());
    std::uninitialized_copy_n(vals.begin(), vals.size(), _data);
    _size = vals.size();
}
IMPL(void)::resize(size_type n, const_reference val) {
    if (n > _size) {
        size_type addition = n - _size;
        ensure_capacity_enough(addition);
        std::uninitialized_fill_n(_data + _size, addition, val);
        _size = n;
    } else if (n < _size) {
        std::destroy(_data + n, _data + _size);
        _size = n;
    }
}
IMPL(void)::resize(size_type n) { resize(n, value_type()); }
IMPL(auto)::at(size_type idx) -> reference {
    return const_cast<reference>(
        static_cast<const ResizingArray &>(*this).at(idx));
}
IMPL(auto)::at(size_type idx) const -> const_reference {
    if (0 < idx && idx <= _size) throw std::out_of_range("Index out of range.");
    return _data[idx];
}

IMPL(inline auto)::begin() noexcept -> iterator { return _data; }
IMPL(inline auto)::end() noexcept -> iterator { return _data + _size; }
IMPL(inline auto)::begin() const noexcept -> const_iterator { return _data; }
IMPL(inline auto)::end() const noexcept -> const_iterator {
    return _data + _size;
}
IMPL(inline auto)::cbegin() const -> const_iterator { return begin(); }
IMPL(inline auto)::cend() const -> const_iterator { return end(); }
IMPL(inline auto)::rbegin() noexcept -> reverse_iterator {
    return reverse_iterator(begin());
}
IMPL(inline auto)::rend() noexcept -> reverse_iterator {
    return reverse_iterator(end());
}
IMPL(inline auto)::rbegin() const noexcept -> const_reverse_iterator {
    return rbegin();
}
IMPL(inline auto)::rend() const noexcept -> const_reverse_iterator {
    return rend();
}
IMPL(inline auto)::crbegin() const noexcept -> const_reverse_iterator {
    return rbegin();
}
IMPL(inline auto)::crend() const noexcept -> const_reverse_iterator {
    return rend();
}
IMPL(inline auto)::front() noexcept -> reference { return *begin(); }
IMPL(inline auto)::front() const noexcept -> const_reference {
    return *begin();
}
IMPL(inline auto)::back() noexcept -> reference { return *(end() - 1); }
IMPL(inline auto)::back() const noexcept -> const_reference {
    return *(end() - 1);
}
IMPL(inline auto)::data() noexcept -> pointer { return _data; }
IMPL(inline auto)::data() const noexcept -> const_pointer { return _data; }
IMPL(inline bool)::empty() const noexcept { return _size == 0; }
IMPL(inline auto)::size() const noexcept -> size_type { return _size; }
IMPL(inline auto)::capacity() const noexcept -> size_type { return _capacity; }
IMPL(inline auto)::max_size() const noexcept -> size_type {
    return alloc_traits::max_size(_alloc);
}
IMPL(inline auto)::get_allocator() const noexcept -> allocator_type {
    return _alloc;
}

#undef IMPL

template <typename V, typename A>
inline bool operator==(const ResizingArray<V, A> &lhs,
                       const ResizingArray<V, A> &rhs) {
    return lhs.size() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename V, typename A>
inline bool operator!=(const ResizingArray<V, A> &lhs,
                       const ResizingArray<V, A> &rhs) {
    return !(lhs == rhs);
}
template <typename V, typename A>
inline bool operator<(const ResizingArray<V, A> &lhs,
                      const ResizingArray<V, A> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                        rhs.end());
}
template <typename V, typename A>
inline bool operator>(const ResizingArray<V, A> &lhs,
                      const ResizingArray<V, A> &rhs) {
    return rhs < lhs;
}
template <typename V, typename A>
inline bool operator<=(const ResizingArray<V, A> &lhs,
                       const ResizingArray<V, A> &rhs) {
    return !(lhs > rhs);
}
template <typename V, typename A>
inline bool operator>=(const ResizingArray<V, A> &lhs,
                       const ResizingArray<V, A> &rhs) {
    return !(lhs < rhs);
}

template <typename V, typename A>
void swap(ResizingArray<V, A> &x, ResizingArray<V, A> &y) {
    x.swap(y);
}

}  // namespace alg
