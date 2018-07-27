#include <gtest/gtest.h>

#include "./test_utility.hpp"
#include "../src/sort/selection_sort.hpp"
#include "../src/vector.hpp"

namespace alg::test {

TEST(SelectionSort, Asc) {
    Vector<int> v = {4, 3, 0, 1, 3, 2, 5}; 
    selection_sort(v.begin(), v.end(), compare_asc<int>);
    EXPECT_TRUE(is_sorted(v.begin(), v.end(), compare_asc<int>));
}
TEST(SelectionSort, Desc) {
    Vector<int> v = {4, 3, 0, 1, 3, 2, 5}; 
    selection_sort(v.begin(), v.end(), compare_desc<int>);
    EXPECT_TRUE(is_sorted(v.begin(), v.end(), compare_desc<int>));
}

}  // namespace alg::test