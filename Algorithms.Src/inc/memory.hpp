#pragma once
#include <iterator>
#include <memory>

namespace alg {
template <typename TAlloc, typename ForwardIt>
void uninit_fill_using_alloc(
    TAlloc &alloc, ForwardIt first, ForwardIt last,
    const typename std::iterator_traits<ForwardIt>::value_type &val) {
    ForwardIt cur = first;
    try {
        for (; cur != last; ++cur) {
            std::allocator_traits<TAlloc>::construct(alloc, cur, val);
        }
    } catch (...) {
        for (; first != cur; ++first) {
            std::allocator_traits<TAlloc>::destroy(alloc,
                                                   std::addressof(*first));
        }
        throw;
    }
}
template <typename TAlloc, typename InputIt, typename ForwardIt>
ForwardIt uninit_copy_using_alloc(TAlloc &alloc, InputIt first, InputIt last,
                                  ForwardIt dfirst) {
    ForwardIt cur = dfirst;
    try {
        for (; first != last; ++first, ++cur) {
            std::allocator_traits<TAlloc>::construct(
                alloc, std::addressof(*cur), *first);
        }
    } catch (...) {
        for (; dfirst != cur; ++dfirst) {
            std::allocator_traits<TAlloc>::destroy(alloc,
                                                   std::addressof(*dfirst));
        }
        throw;
    }
    return cur;
}
template <typename TAlloc, typename InputIt, typename ForwardIt>
ForwardIt uninit_move_using_alloc(TAlloc &alloc, InputIt first, InputIt last,
                                  ForwardIt dfirst) {
    ForwardIt cur = dfirst;
    try {
        for (; first != last; ++first, ++cur) {
            std::allocator_traits<TAlloc>::construct(
                alloc, std::addressof(*cur), std::move(*first));
        }
    } catch (...) {
        for (; dfirst != cur; ++dfirst) {
            std::allocator_traits<TAlloc>::destroy(alloc,
                                                   std::addressof(*dfirst));
        }
        throw;
    }
    return cur;
}
template <typename TAlloc, typename TSize, typename ForwardIt>
ForwardIt uninit_fill_n_using_alloc(
    TAlloc &alloc, ForwardIt first, TSize n,
    const typename std::iterator_traits<ForwardIt>::value_type &val) {
    ForwardIt cur = first;
    try {
        for (; n > 0; ++cur, --n) {
            std::allocator_traits<TAlloc>::construct(alloc,
                                                     std::addressof(*cur), val);
        }
    } catch (...) {
        for (; first != cur; ++first) {
            std::allocator_traits<TAlloc>::destroy(alloc,
                                                   std::addressof(*first));
        }
        throw;
    }
    return cur;
}
template <typename TAlloc, typename InputIt, typename TSize, typename ForwardIt>
ForwardIt uninit_copy_n_using_alloc(TAlloc &alloc, InputIt first, TSize n,
                                    ForwardIt dfirst) {
    ForwardIt cur = dfirst;
    try {
        for (; n > 0; ++first, (void)++cur, --n) {
            std::allocator_traits<TAlloc>::construct(
                alloc, std::addressof(*cur), *first);
        }
    } catch (...) {
        for (; dfirst != cur; ++dfirst) {
            std::allocator_traits<TAlloc>::destroy(alloc,
                                                   std::addressof(*dfirst));
        }
        throw;
    }
    return cur;
}
template <typename TAlloc, typename InputIt, typename TSize, typename ForwardIt>
std::pair<InputIt, ForwardIt> uninit_move_n_using_alloc(TAlloc &alloc,
                                                        InputIt first, TSize n,
                                                        ForwardIt dfirst) {
    ForwardIt current = dfirst;
    try {
        for (; n > 0; ++first, (void)++current, --n) {
            std::allocator_traits<TAlloc>::construct(
                alloc, std::addressof(*current), *first);
        }
    } catch (...) {
        for (; dfirst != current; ++dfirst) {
            std::allocator_traits<TAlloc>::destroy(alloc,
                                                   std::addressof(*dfirst));
        }
        throw;
    }
    return {first, current};
}
template <typename TAlloc, typename ForwardIt>
void destroy_using_alloc(TAlloc &alloc, ForwardIt first, ForwardIt last) {
    for (; first != last; ++first) {
        std::allocator_traits<TAlloc>::destroy(alloc, std::addressof(*first));
    }
}
template <typename TAlloc, typename TSize, typename ForwardIt>
void destroy_n_using_alloc(TAlloc &alloc, ForwardIt iter, TSize n) {
    for (; n > 0; ++iter, --n) {
        std::allocator_traits<TAlloc>::destroy(alloc, std::addressof(*iter));
    }
}
template <typename TAlloc, typename BidIt1, typename BidIt2>
BidIt2 uninit_move_backward_using_alloc(TAlloc &alloc, BidIt1 first,
                                        BidIt1 last, BidIt2 dlast) {
    BidIt2 cur = dlast;
    try {
        while (first != last) {
            std::allocator_traits<TAlloc>::construct(
                alloc, std::addressof(*(--cur)), std::move(*(--last)));
        }
    } catch (...) {
        for (++cur; cur != dlast; ++cur) {
            std::allocator_traits<TAlloc>::destroy(alloc, std::addressof(*cur));
        }
        throw;
    }
    return dlast;
}

}  // namespace alg
