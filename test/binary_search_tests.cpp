#include <iostream>

#include <gtest/gtest.h>

#include "../src/search/binary_search.hpp"

namespace alg::search::test {

TEST(BinarySearchTest, Contains) {
    ds::Vector<int> v = {1, 2, 3, 4};
    ASSERT_EQ(1, binary_search<int>(2, v));
}
TEST(BinarySearchTest, NotContain) {
    ds::Vector<int> v = {1, 2, 3, 4};
    ASSERT_EQ(-1, binary_search<int>(5, v));
}

}  // namespace alg::search::test
