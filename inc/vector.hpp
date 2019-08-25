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
};

// -----------------------------------------------------------------------------

#define IMPL(declaration...)          \
    template <typename V, typename A> \
    declaration Vector<V, A>

IMPL(inline auto)::make_iter(const_iterator iter) -> iterator {
    return this->data() + (iter - this->data());
}

IMPL(template <typename... TArgs> auto)::emplace(const_iterator pos,
                                                 TArgs &&... args) -> iterator {
    iterator ne = std::next(this->end());
    iterator mp = make_iter(pos);
    this->ensure_capacity_enough(this->size() + 1);
    uninit_move_backward_using_alloc(this->alloc(), mp, this->end(), ne);
    this->construct(make_iter(pos), std::forward<TArgs>(args)...);
    this->set_size(this->size() + 1);
    return mp;
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
    static_assert(
        std::is_base_of_v<
            std::input_iterator_tag,
            typename std::iterator_traits<InputIt>::iterator_category>,
        "InputIt must be an input iterator.");
    auto distance = std::distance(first, last);
    size_type ns = this->size() + distance;
    iterator ne = this->end() + distance;
    iterator mp = make_iter(pos);
    this->ensure_capacity_enough(ns);
    uninit_move_backward_using_alloc(this->alloc(), mp, this->end(), ne);
    uninit_copy_using_alloc(this->alloc(), first, last, mp);
    this->set_size(ns);
    return mp;
}

IMPL(auto)::insert(const_iterator pos, std::initializer_list<value_type> vals)
    -> iterator {
    size_type ns = this->size() + vals.size();
    iterator ne = this->end() + vals.size();
    iterator mp = make_iter(pos);
    this->ensure_capacity_enough(ns);
    uninit_move_backward_using_alloc(this->alloc(), mp, this->end(), ne);
    uninit_copy_using_alloc(this->alloc(), vals.begin(), vals.end(), mp);
    this->set_size(ns);
    return mp;
}

IMPL(auto)::insert(const_iterator pos, size_type count, const_reference val)
    -> iterator {
    size_type ns = this->size() + count;
    iterator ne = this->end() + count;
    iterator mp = make_iter(pos);
    this->ensure_capacity_enough(this->size() + count);
    uninit_move_backward_using_alloc(this->alloc(), mp, this->end(), ne);
    uninit_fill_n_using_alloc(this->alloc(), mp, count, val);
    this->set_size(ns);
    return make_iter(pos);
}

IMPL(auto)::erase(const_iterator pos) -> iterator {
    iterator mp = make_iter(pos);
    this->destroy(mp);
    uninit_move_using_alloc(this->alloc(), std::next(mp), this->end(), mp);
    this->set_size(this->size() - 1);
    return mp;
}

IMPL(auto)::erase(const_iterator first, const_iterator last) -> iterator {
    iterator mf = make_iter(first), ml = make_iter(last);

    std::destroy(mf, ml);
    uninit_move_using_alloc(this->alloc(), ml, this->end(), mf);
    this->set_size(this->size() - std::distance(first, last));
    return mf;
}

}  // namespace alg
