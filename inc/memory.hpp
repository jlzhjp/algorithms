#pragma once
#include <iterator>

namespace alg {

template <typename BidIt1, typename BidIt2>
BidIt2 uninitialized_move_backward(BidIt1 first, BidIt1 last, BidIt2 d_last) {
    using value_type = typename std::iterator_traits<BidIt2>::value_type;
    using d_value_type = typename std::iterator_traits<BidIt2>::value_type;
    static_assert(std::is_convertible_v<
                      typename std::iterator_traits<BidIt1>::iterator_category,
                      std::bidirectional_iterator_tag>,
                  "BidIt1 must be a bidirectional iterator type.");
    static_assert(std::is_convertible_v<
                      typename std::iterator_traits<BidIt2>::iterator_category,
                      std::bidirectional_iterator_tag>,
                  "BidIt2 must be a bidirectional iterator type.");
    static_assert(std::is_constructible_v<d_value_type, value_type &&>);
    auto cur = d_last;
    try {
        while (first != last) {
            ::new (static_cast<void *>(std::addressof(*(--cur))))
                d_value_type(std::move(*(--last)));
        }
    } catch (...) {
        for (++cur; cur != d_last; ++cur) {
            cur->~d_value_type();
        }
        throw;
    }
    return d_last;
}

}  // namespace alg
