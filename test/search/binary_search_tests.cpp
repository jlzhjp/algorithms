#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include "../../src/search/binary_search.hpp"

namespace algo::search::test {

TEST(BinarySearchTest, Contains) {
    std::vector<int> v = { 1, 2, 3, 4, };
    ASSERT_EQ(1, algo::binary_search<int>(2, v));
}
TEST(BinarySearchTest, NotContain) {
    std::vector<int> v = { 1, 2, 3, 4, };
    ASSERT_EQ(-1, algo::binary_search<int>(5, v));
}

} // namespace algo::test
