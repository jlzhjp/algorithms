#ifndef __TEST_UTILITY_HPP__
#define __TEST_UTILITY_HPP__

#include <algorithm>
#include <iterator>
#include <random>
#include <string>

#include "../src/vector.hpp"

namespace alg::test {

template <typename ForwardIt, typename TComparer>
bool is_sorted(ForwardIt first, ForwardIt last, TComparer comp) {
    ForwardIt prev = first;
    for (++first; first != last; ++first, ++prev) {
        if (comp(*prev, *first) > 0) return false;
    }
    return true;
}
template <typename InputIt>
std::string gen_content_str(InputIt first, InputIt last) {
    std::string result = "{ ";
    std::for_each(
        first, last,
        [&result](typename std::iterator_traits<InputIt>::reference val) {
            result += (std::to_string(val) + ", ");
        });
    return result + "}";
}
// 生成 +-100 以内的随机数序列
Vector<int> gen_random_seq(Vector<int>::size_type n) {
    Vector<int> result;
    std::random_device rand;
    for (auto i = n; i != 0; --i) {
        unsigned long s = rand() % 2, num = rand() % 100;
        result.push_back(s == 0 ? -num : num);
    }
    return result;
}
}  // namespace alg::test

#endif
