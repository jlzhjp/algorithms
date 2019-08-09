#pragma once

#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <type_traits>

#include "./vector.hpp"

namespace alg {

template <typename T, typename TContainer = Vector<T>>
class Stack {
public:
    using container_type = TContainer;
    using allocator_type = typename container_type::allocator_type;
    using value_type = typename container_type::value_type;
    using size_type = typename container_type::size_type;
    using reference = typename container_type::reference;
    using const_reference = typename container_type::const_reference;

public:
    explicit Stack() = default;
    explicit Stack(container_type &container) : _container(container) {}
    explicit Stack(container_type &&container)
        : _container(std::forward<container_type>(container)) {}
    Stack(std::initializer_list<value_type> values) : _container(values) {}
    Stack(Stack &) = default;
    Stack(Stack &&) = default;

public:
    Stack &operator=(Stack &) = default;
    Stack &operator=(Stack &&) = default;

public:
    value_type pop() {
        value_type result = _container.back();
        _container.pop_back();
        return result;
    }
    void push(value_type &value) { _container.push_back(value); }
    void push(value_type &&value) {
        _container.push_back(std::forward<value_type>(value));
    }
    void swap(Stack &rhs) { _container.swap(rhs._container); }

public:
    bool empty() const { return _container.empty(); }
    value_type &top() noexcept { return _container.back(); }
    const value_type &top() const noexcept { return _container.back(); }
    size_type size() const { return _container.size(); }
    size_type capacity() { return _container.capacity(); }

private:
    container_type _container;

    template <typename V, typename C>
    friend bool operator==(const Stack<V, C> &, const Stack<V, C> &);
    template <typename V, typename C>
    friend bool operator<(const Stack<V, C> &, const Stack<V, C> &);
};

template <typename V, typename C>
inline bool operator==(const Stack<V, C> &lhs, const Stack<V, C> &rhs) {
    return lhs._container == rhs._container;
}
template <typename V, typename C>
inline bool operator!=(const Stack<V, C> &lhs, const Stack<V, C> &rhs) {
    return !(lhs == rhs);
}
template <typename V, typename C>
inline bool operator<(const Stack<V, C> &lhs, const Stack<V, C> &rhs) {
    return lhs._container < rhs._container;
}
template <typename V, typename C>
inline bool operator>(const Stack<V, C> &lhs, const Stack<V, C> &rhs) {
    return rhs < lhs;
}
template <typename V, typename C>
inline bool operator<=(const Stack<V, C> &lhs, const Stack<V, C> &rhs) {
    return !(lhs > rhs);
}
template <typename V, typename C>
inline bool operator>=(const Stack<V, C> &lhs, const Stack<V, C> &rhs) {
    return !(lhs < rhs);
}

}  // namespace alg
namespace std {

template <typename V, typename C>
void swap(alg::Stack<V, C> &lhs, alg::Stack<V, C> &rhs) {
    lhs.swap(rhs);
}

}  // namespace std
