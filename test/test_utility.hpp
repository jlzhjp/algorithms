#ifndef __TEST_UTILITY_HPP__
#define __TEST_UTILITY_HPP__

namespace alg::test {

template <typename ForwardIt, typename TComparer>
bool is_sorted(ForwardIt first, ForwardIt last, TComparer comp){
    ForwardIt prev = first;
    for (++first; first != last; ++first) {
        if (comp(*prev, *first) > 0) return false;
        prev = first;
    }
    return true;
}

}
#endif