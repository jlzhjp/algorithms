#pragma once

#include <initializer_list>

#include "memory.hpp"
#include "resizing_array.hpp"

namespace alg {

template <typename V, typename A = std::allocator<V>>
class Vector : public ResizingArray<V, A> {
private:
    using Base = ResizingArray<V, A>;

public:
    using typename Base::const_iterator;
    using typename Base::const_pointer;
    using typename Base::const_reference;
    using typename Base::difference_type;
    using typename Base::iterator;
    using typename Base::pointer;
    using typename Base::reference;
    using typename Base::size_type;
    using typename Base::value_type;

public:
    using Base::ResizingArray;

public:
    template <typename... TArgs>
    iterator emplace(const_iterator pos, TArgs &&... args);

    iterator insert(const_iterator pos, const_reference val);
    iterator insert(const_iterator pos, value_type &&val);
    iterator insert(const_iterator pos, size_type count, const_reference val);
    template <typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last);
    iterator insert(const_iterator pos, std::initializer_list<value_type> vals);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

private:
    iterator make_iter(const_iterator iter);
    template <typename BidIt1, typename BidIt2>
    BidIt2 uninitialized_move_backward(BidIt1 first, BidIt1 last,
                                       BidIt2 d_last);
};

#define IMPL(declaration...)          \
    template <typename V, typename A> \
    declaration Vector<V, A>

IMPL(inline auto)::make_iter(const_iterator iter) -> iterator {
    return this->data() + (iter - this->data());
}

IMPL(template <typename... TArgs> auto)::emplace(const_iterator pos,
                                                 TArgs &&... args) -> iterator {
    iterator ne = std::next(this->end());
    this->ensure_capacity_enough(this->size() + 1);
    alg::uninitialized_move_backward(make_iter(pos), this->end(), ne);
    this->construct(make_iter(pos), std::forward<TArgs>(args)...);
    this->set_size(this->size() + 1);
    return make_iter(pos);
}

IMPL(inline auto)::insert(const_iterator pos, const_reference val) -> iterator {
    return emplace(pos, val);
}

IMPL(inline auto)::insert(const_iterator pos, value_type &&val) -> iterator {
    return emplace(pos, std::forward<value_type>(val));
}

IMPL(template <typename InputIt> auto)::insert(const_iterator pos,
                                               InputIt first, InputIt last)
    -> iterator {
    static_assert(std::is_assignable_v<
                      typename std::iterator_traits<InputIt>::iterator_category,
                      std::input_iterator_tag>,
                  "InputIt must be an input iterator.");
    auto distance = std::distance(first, last);
    size_type ns = this->size() + distance;
    iterator ne = this->end() + distance;
    this->ensure_capacity_enough(ns);
    alg::uninitialized_move_backward(make_iter(pos), this->end(), ne);
    std::uninitialized_copy(first, last, make_iter(pos));
    this->set_size(ns);
    return make_iter(pos);
}

IMPL(auto)::insert(const_iterator pos, std::initializer_list<value_type> vals)
    -> iterator {
    size_type ns = this->size() + vals.size();
    iterator ne = this->end() + vals.size();
    this->ensure_capacity_enough(ns);
    alg::uninitialized_move_backward(make_iter(pos), this->end(), ne);
    std::uninitialized_copy(vals.begin(), vals.end(), make_iter(pos));
    this->set_size(ns);
    return make_iter(pos);
}

IMPL(auto)::insert(const_iterator pos, size_type count, const_reference val)
    -> iterator {
    size_type ns = this->size() + count;
    iterator ne = this->end() + count;
    this->ensure_capacity_enough(this->size() + count);
    alg::uninitialized_move_backward(make_iter(pos), this->end(), ne);
    std::uninitialized_fill_n(make_iter(pos), count, val);
    this->set_size(ns);
    return make_iter(pos);
}

IMPL(auto)::erase(const_iterator pos) -> iterator {
    this->destroy(make_iter(pos));
    std::uninitialized_move(std::next(make_iter(pos)), this->end(),
                            make_iter(pos));
    this->set_size(this->size() - 1);
    return make_iter(pos);
}

IMPL(auto)::erase(const_iterator first, const_iterator last) -> iterator {
    std::destroy(make_iter(first), make_iter(last));
    std::uninitialized_move(make_iter(last), this->end(), make_iter(first));
    this->set_size(this->size() - std::distance(first, last));
    return make_iter(first);
}

}  // namespace alg
