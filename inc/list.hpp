#pragma once

#include <memory>

namespace alg {

namespace internal {

template <typename T, bool IsConst>
struct IteratorTypeTraits;
template <typename T>
struct IteratorTypeTraits<T, true> {
    using value_type = T;
    using reference = const T &;
    using pointer = const T *;
};
template <typename T>
struct IteratorTypeTraits<T, false> {
    using value_type = T;
    using reference = T &;
    using pointer = T *;
};

}  // namespace internal

template <typename T, typename TAlloc = std::allocator<T>>
class List {
    using AllocTraits = std::allocator_traits<TAlloc>;
    struct Node;
    template <bool IsConst>
    struct ListIterator;

public:
    using allocator_type = TAlloc;
    using value_type = typename AllocTraits::value_type;
    using pointer = typename AllocTraits::pointer;
    using size_type = typename AllocTraits::size_type;
    using difference_type = typename AllocTraits::difference_type;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = ListIterator<false>;
    using const_iterator = ListIterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    using NodeAlloc = typename AllocTraits::template rebind_alloc<Node>;
    using NodeAllocTraits = std::allocator_traits<NodeAlloc>;
    using NodePointer = typename NodeAllocTraits::pointer;

private:
    struct Node {
        Node() = default;
        Node(const_reference d, Node *p = nullptr, Node *n = nullptr)
            : data(d), prev(p), next(n) {}
        Node(value_type &&d, Node *p = nullptr, Node *n = nullptr)
            : data(std::forward<value_type>(d)), prev(p), next(n) {}
        T data;
        Node *prev;
        Node *next;
    };
    template <typename... TArgs>
    NodePointer new_node(Node *prev, Node *next, TArgs &&... args);

    template <bool IsConst>
    struct ListIterator {
        using value_type =
            typename internal::IteratorTypeTraits<T, IsConst>::value_type;
        using reference =
            typename internal::IteratorTypeTraits<T, IsConst>::reference;
        using pointer =
            typename internal::IteratorTypeTraits<T, IsConst>::pointer;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        template <bool _IsConst = IsConst,
                  typename = std::enable_if_t<_IsConst>>
        ListIterator(const ListIterator<false> &rhs) : _node(rhs._node) {}
        ListIterator(const ListIterator &rhs) = default;
        ListIterator(ListIterator &&rhs) = default;
        ListIterator &operator=(const ListIterator &rhs) = default;
        ListIterator &operator=(ListIterator &&rhs) = default;

        reference operator*() const { return _node->data; }
        reference operator->() const { return _node; }
        ListIterator &operator++() {
            _node = _node->next;
            return *this;
        }
        ListIterator &operator--() {
            _node = _node->prev;
            return *this;
        }
        ListIterator operator++(int) {
            ListIterator tmp(*this);
            _node = _node->next;
            return tmp;
        }
        ListIterator operator--(int) {
            ListIterator tmp(*this);
            _node = _node->prev;
            return tmp;
        }
        bool operator==(const ListIterator &rhs) {
            return rhs._node == this->_node;
        }
        bool operator!=(const ListIterator &rhs) { return !operator==(rhs); }

    private:
        explicit ListIterator(Node *n) : _node(n) {}
        Node *get_node() const { return this->_node; }
        Node *_node;

        friend class List<T, TAlloc>;
    };

public:
    List();
    explicit List (size_type count, const T &val = T());
    List(std::initializer_list<value_type> vals);
    List(const List &rhs);
    List(List &&rhs) noexcept;
    List &operator=(const List &rhs);
    List &operator=(List &&rhs);
    ~List();

public:
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

public:
    void clear() noexcept;
    iterator insert(const_iterator pos, const_reference val);
    iterator insert(const_iterator pos, value_type &&val);
    iterator insert(const_iterator pos, size_type count, const T &val);
    template <typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last);
    iterator insert(const_iterator pos, std::initializer_list<value_type> vals);
    template <typename... TArgs>
    iterator emplace(const_iterator pos, TArgs &&... args);
    void push_back(const_reference val);
    void push_back(value_type &&val);
    template <typename... TArgs>
    reference emplace_back(TArgs &&... args);
    void pop_back();
    void push_front(const_reference val);
    void push_front(value_type &&val);
    template <typename... TArgs>
    reference emplace_front(TArgs &&... args);
    void pop_front();
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void resize(size_type count);
    void resize(size_type count, const_reference val);
    void swap(List &other) noexcept;

public:
    void merge(List &&other);
    template <typename TComparer>
    void merge(List &&other, TComparer comp);
    void splice(const_iterator pos, List &&other);
    void splice(const_iterator pos, List &&other, const_iterator it);
    void splice(const_iterator pos, List &&other, const_iterator first,
                const_iterator last);
    size_type remove(const_reference val);
    template <typename TUnaryPredicate>
    size_type remove_if(const_reference val, TUnaryPredicate p);
    void reverse() noexcept;
    size_type unique();
    template <typename TBinaryPredicate>
    size_type unique(TBinaryPredicate p);
    void sort();
    template <typename TComparer>
    void sort(TComparer comp);

public:
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

private:
    iterator make_iter(const_iterator iter);

private:
    Node *_entry = nullptr;
    size_type _size = 0;
    allocator_type _alloc;
    NodeAlloc _nodeAlloc;
};

#define IMPL(declaration...)               \
    template <typename T, typename TAlloc> \
    declaration List<T, TAlloc>

IMPL(template <typename... TArgs>
     auto)::new_node(Node *prev, Node *next, TArgs &&... args) -> NodePointer {
    NodePointer p = NodeAllocTraits::allocate(_nodeAlloc, 1);
    p->prev = prev;
    p->next = next;
    AllocTraits::construct(_alloc, &p->data, std::forward<TArgs>(args)...);
    return p;
}

IMPL()::List() : _entry(new_node(nullptr, nullptr, value_type())) {
    _entry->prev = _entry;
    _entry->next = _entry;
}
IMPL()::List(size_type count, const T &val) : List() {
    for (int i = 0; i < count; ++i) {
        push_back(val);
    }
}
IMPL()::List(std::initializer_list<value_type> vals) : List() {
    for (auto &val : vals) {
        push_back(val);
    }
}
IMPL()::List(const List &rhs) : List() {
    for (auto &v : rhs) {
        push_back(v);
    }
}
IMPL()::List(List &&rhs) noexcept : List() {
   for (auto &v : rhs) {
       push_back(std::move(v));
   }
   rhs.resize(0);
}
IMPL(auto)::operator=(const List &rhs) -> List & {
    List tmp(rhs);
    swap(tmp);
}
IMPL(auto)::operator=(List &&rhs) -> List & {
    List tmp(std::move(rhs));
    swap(tmp);
}
IMPL(void)::swap(List &other) noexcept {
    if (other == *this) return;
    std::swap(_size, other._size);
    std::swap(_entry, other._entry);
    std::swap(_alloc, other._alloc);
    std::swap(_nodeAlloc, other._nodeAlloc);
}
IMPL()::~List() {
    Node *p = _entry;
    do {
        Node *pp = p->next;
        NodeAllocTraits::destroy(_nodeAlloc, p);
        NodeAllocTraits::deallocate(_nodeAlloc, p, 1);
        p = pp;
    } while (p != _entry);
}
IMPL(inline auto)::make_iter(const_iterator iter) -> iterator {
    return iterator(iter.get_node());
}
IMPL(inline auto)::begin() noexcept -> iterator {
    return iterator(_entry->next);
}
IMPL(inline auto)::end() noexcept -> iterator { return iterator(_entry); }
IMPL(inline auto)::begin() const noexcept -> const_iterator {
    return const_iterator(_entry->next);
}
IMPL(inline auto)::end() const noexcept -> const_iterator {
    return const_iterator(_entry);
}
IMPL(inline auto)::cbegin() const noexcept -> const_iterator { return begin(); }
IMPL(inline auto)::cend() const noexcept -> const_iterator { return end(); }
IMPL(inline auto)::rend() noexcept -> reverse_iterator {
    return reverse_iterator(--end());
}
IMPL(inline auto)::rbegin() noexcept -> reverse_iterator {
    return reverse_iterator(--begin()());
}
IMPL(inline auto)::crbegin() const noexcept -> const_reverse_iterator {
    return reverse_iterator(--end());
}
IMPL(inline auto)::crend() const noexcept -> const_reverse_iterator {
    return reverse_iterator(--begin());
}
IMPL(inline auto)::size() const noexcept -> size_type { return _size; }
IMPL(inline auto)::max_size() const noexcept -> size_type {
    return NodeAllocTraits::max_size();
}
IMPL(inline bool)::empty() const noexcept { return _entry->next == _entry; }
IMPL(template <typename... TArgs> auto)::emplace(const_iterator pos,
                                                 TArgs &&... args) -> iterator {
    Node *ptn = pos.get_node();
    Node *ptp = ptn->prev;
    ptp->next = new_node(ptp, ptn, std::forward<TArgs>(args)...);
    ptn->prev = ptp->next;
    ++_size;
    return iterator(ptp->next);
}
IMPL(template <typename... TArgs> inline auto)::emplace_back(TArgs &&... args)
    -> reference {
    return *emplace(end(), std::forward<TArgs>(args)...);
}
IMPL(template <typename... TArgs> inline auto)::emplace_front(TArgs &&... args)
    -> reference {
    return *emplace(begin(), std::forward<TArgs>(args)...);
}
IMPL(auto)::insert(const_iterator pos, const_reference val) -> iterator {
    return emplace(pos, val);
}
IMPL(auto)::insert(const_iterator pos, value_type &&val) -> iterator {
    return emplace(pos, std::forward<value_type>(val));
}
IMPL(auto)::insert(const_iterator pos, std::initializer_list<value_type> vals)
    -> iterator {
    Node *prev = pos.get_node()->prev;
    for (auto &val : vals) {
        emplace(pos++, val);
    }
    return iterator(prev->next);
}
IMPL(auto)::insert(const_iterator pos, size_type count, const T &val)
    -> iterator {
    Node *prev = pos.get_node()->prev;
    for (size_type i = 0; i < count; ++i) {
        emplace(pos++, val);
    }
    return iterator(prev->next);
}
IMPL(void)::push_back(const_reference val) { insert(end(), val); }
IMPL(void)::push_back(value_type &&val) {
    insert(end(), std::forward<value_type>(val));
}
IMPL(void)::push_front(const_reference val) { insert(begin(), val); }
IMPL(void)::push_front(value_type &&val) {
    insert(begin(), std::forward<value_type>(val));
}
IMPL(auto)::erase(const_iterator pos) -> iterator {
    Node *n = pos.get_node();
    Node *np = n->prev;
    Node *nn = n->next;
    NodeAllocTraits::deallocate(_nodeAlloc, n, 1);
    np->next = nn;
    return Node(nn);
}
IMPL(auto)::erase(const_iterator first, const_iterator last) -> iterator {
    if (first == last) return make_iter(first);
    Node *nf = first.get_node(), *nl = last.get_node(), *np = nf->prev;
    for (Node *n = nf; n != nl; ++n) {
        NodeAllocTraits::deallocate(_nodeAlloc, n, 1);
    }
    np->next = nl;
    return make_iter(last);
}
IMPL(void)::pop_front() { erase(begin()); }
IMPL(void)::resize(size_type count) { resize(count, value_type()); }
IMPL(void)::resize(size_type count, const_reference val) {
    if (count > _size) {
        for (int i = _size; i != count; ++i) {
            push_back(val);
        }
    } else if (count < _size) {
        for (int i = count; i != _size; ++i) {
            pop_back();
        }
    }
}

}  // namespace alg
