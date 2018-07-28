#ifndef __TEST_UTILITY_HPP__
#define __TEST_UTILITY_HPP__

#include <algorithm>
#include <iterator>
#include <string>

namespace alg::test {

template <typename ForwardIt, typename TComparer>
bool is_sorted(ForwardIt first, ForwardIt last, TComparer comp) {
    ForwardIt prev = first;
    for (first = std::next(first); first != last; ++first) {
        if (comp(*prev, *first) > 0) return false;
        prev = first;
    }
    return true;
}
template <typename InputIt>
std::string get_content_str(InputIt first, InputIt last) {
    std::string result = "{ ";
    std::for_each(
        first, last,
        [&result](typename std::iterator_traits<InputIt>::reference val) {
            result += (std::to_string(val) + ", ");
        });
    return result + "}";
}

}  // namespace alg::test

#endif
