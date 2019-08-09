#include <gtest/gtest.h>
#include "search/binary_search.hpp"

#include <iostream>

#include "vector.hpp"

namespace alg::test {

TEST(BinarySearchTest, Contains) {
    Vector<int> v = {1, 2, 3, 4};
    ASSERT_EQ(v.begin() + 1, binary_search(v.begin(), v.end(), 2));
}
TEST(BinarySearchTest, NotContain) {
    Vector<int> v = {1, 2, 3, 4};
    ASSERT_EQ(v.end(), binary_search(v.begin(), v.end(), 5));
}

}  // namespace alg::test
