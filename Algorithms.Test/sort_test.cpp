#include <gtest/gtest.h>

#include "array.hpp"
#include "sort/insertion_sort.hpp"
#include "sort/merge_sort.hpp"
#include "sort/quick_sort.hpp"
#include "sort/selection_sort.hpp"
#include "sort/shell_sort.hpp"
#include "test_utility.hpp"

namespace alg::test {

TEST(SelectionSort, Asc) {
    Array<int, 9> arr;
    gen_random_seq(arr.begin(), arr.end());
    selection_sort(arr.begin(), arr.end());
    EXPECT_TRUE(is_sorted(arr.begin(), arr.end()))
        << gen_content_str(arr.begin(), arr.end());
}
TEST(SelectionSort, Desc) {
    Array<int, 9> arr;
    gen_random_seq(arr.begin(), arr.end());
    selection_sort(arr.begin(), arr.end(), compare_desc<int>);
    EXPECT_TRUE(is_sorted(arr.begin(), arr.end(), compare_desc<int>))
        << gen_content_str(arr.begin(), arr.end());
}
TEST(InsertionSort, Asc) {
    Array<int, 9> arr;
    gen_random_seq(arr.begin(), arr.end());
    insertion_sort(arr.begin(), arr.end());
    EXPECT_TRUE(is_sorted(arr.begin(), arr.end()))
        << gen_content_str(arr.begin(), arr.end());
}
TEST(InsertionSort, Desc) {
    Array<int, 9> arr;
    gen_random_seq(arr.begin(), arr.end());
    insertion_sort(arr.begin(), arr.end(), compare_desc<int>);
    EXPECT_TRUE(is_sorted(arr.begin(), arr.end(), compare_desc<int>))
        << gen_content_str(arr.begin(), arr.end());
}
TEST(ShellSort, Asc) {
    Array<int, 9> arr;
    gen_random_seq(arr.begin(), arr.end());
    shell_sort(arr);
    EXPECT_TRUE(is_sorted(arr.begin(), arr.end()))
        << gen_content_str(arr.begin(), arr.end());
}
TEST(ShellSort, Desc) {
    Array<int, 9> arr;
    gen_random_seq(arr.begin(), arr.end());
    shell_sort(arr, compare_desc<int>);
    EXPECT_TRUE(is_sorted(arr.begin(), arr.end(), compare_desc<int>))
        << gen_content_str(arr.begin(), arr.end());
}
TEST(MergeSort, Asc) {
    Array<int, 9> arr;
    gen_random_seq(arr.begin(), arr.end());
    merge_sort(arr);
    EXPECT_TRUE(is_sorted(arr.begin(), arr.end()))
        << gen_content_str(arr.begin(), arr.end());
}
TEST(MergeSort, Desc) {
    Array<int, 9> arr;
    gen_random_seq(arr.begin(), arr.end());
    merge_sort(arr, compare_desc<int>);
    EXPECT_TRUE(is_sorted(arr.begin(), arr.end(), compare_desc<int>))
        << gen_content_str(arr.begin(), arr.end());
}
TEST(QuickSort, Asc) {
    Array<int, 9> arr;
    gen_random_seq(arr.begin(), arr.end());
    quick_sort(arr.begin(), arr.end());
    EXPECT_TRUE(is_sorted(arr.begin(), arr.end()))
        << gen_content_str(arr.begin(), arr.end());
}
TEST(QuickSort, Desc) {
    Array<int, 9> arr;
    gen_random_seq(arr.begin(), arr.end());
    quick_sort(arr.begin(), arr.end(), compare_desc<int>);
    EXPECT_TRUE(is_sorted(arr.begin(), arr.end(), compare_desc<int>))
        << gen_content_str(arr.begin(), arr.end());
}

}  // namespace alg::test
