#include "resizing_array.hpp"
#include <gtest/gtest.h>

#include <iostream>
#include <list>

namespace alg::test {

TEST(ResizingArray, Constructor) {
    ResizingArray<std::string> a1;
    EXPECT_EQ(0, a1.size());
    ResizingArray<std::string> a2 = {"A", "B", "C"};
    EXPECT_EQ(3, a2.size());
    EXPECT_EQ("A", a2[0]);
    EXPECT_EQ("B", a2[1]);
    EXPECT_EQ("C", a2[2]);
    ResizingArray<std::string> v3(10);
    EXPECT_EQ(10, v3.capacity());
    EXPECT_EQ(0, v3.size());
}
TEST(ResizingArray, CopyConstructor) {
    ResizingArray<std::string> a1 = {"A", "B", "C"};
    ResizingArray<std::string> a2 = a1;
    EXPECT_EQ(a1, a2);
}
TEST(ResizingArray, MoevConstructor) {
    ResizingArray<std::string> a1 = {"A", "B", "C"};
    ResizingArray<std::string> a2 = std::move(a1);
    ResizingArray<std::string> expected({"A", "B", "C"});
    EXPECT_EQ(expected, a2);
}
TEST(ResizingArray, CopyAssignment) {
    ResizingArray<std::string> a1 = {"A", "B", "C"};
    ResizingArray<std::string> a2 = {"D", "E"};
    a2 = a1;
    EXPECT_EQ(a1, a2);
}
TEST(ResizingArray, MoveAssignment) {
    ResizingArray<std::string> a1 = {"A", "B", "C"};
    ResizingArray<std::string> a2 = {"D", "E"};
    a2 = std::move(a1);
    ResizingArray<std::string> expected = {"A", "B", "C"};
    EXPECT_EQ(expected, a2);
}
TEST(ResizingArray, Assign) {
    ResizingArray<std::string> a;

    a.assign(30, "A");
    EXPECT_EQ(a.size(), 30);
    for (auto val : a) {
        EXPECT_EQ("A", val);
    }

    std::list<std::string> lst = {"A", "B", "C"};
    a.assign(lst.begin(), lst.end());
    EXPECT_TRUE(std::equal(a.begin(), a.end(), lst.begin(), lst.end()));

    std::initializer_list<std::string> l = {"A", "B", "C"};
    a.assign(l.begin(), l.end());
    EXPECT_TRUE(std::equal(a.begin(), a.end(), l.begin(), l.end()));
}
TEST(ResizingArray, EmplaceBack) {
    ResizingArray<std::string> a;
    std::string str = "ABC";
    a.emplace_back(str.begin(), str.end());
    a.emplace_back(str.begin(), str.end());
    EXPECT_EQ("ABC", a[0]);
    EXPECT_EQ("ABC", a[1]);
}
TEST(ResizingArray, Clear) {
    ResizingArray<std::string> a({"A", "B", "C"});
    a.clear();
    EXPECT_EQ(0, a.size());
}
TEST(ResizingArray, Resize) {
    ResizingArray<std::string> a({"A", "B", "C"});
    a.resize(5, "D");
    ResizingArray<std::string> expected = {"A", "B", "C", "D", "D"};
    EXPECT_EQ(expected, a);
    a.resize(2);
    expected = {"A", "B"};
    EXPECT_EQ(expected, a);
}
TEST(ResizingArray, Reserve) {
    ResizingArray<std::string> a = {"A", "B"};
    auto cap = a.capacity();
    a.reserve(0);
    EXPECT_EQ(cap, a.capacity());
    a.reserve(100);
    EXPECT_EQ(100, a.capacity());
}
TEST(ResizingArray, Swap) {
    ResizingArray<std::string> a1 = {"A", "B", "C"};
    ResizingArray<std::string> a2 = {"D", "E"};
    a1.swap(a2);
    ResizingArray<std::string> a1_expected = {"D", "E"};
    ResizingArray<std::string> a2_expected = {"A", "B", "C"};
    EXPECT_EQ(a1_expected, a1);
    EXPECT_EQ(a2_expected, a2);
}
TEST(ResizingArray, Stress) {
    ResizingArray<std::string> a(0);
    for (int i = 0; i <= 100; ++i) {
        a.push_back(std::to_string(i));
    }
    for (int i = 100; i >= 0; --i) {
        EXPECT_EQ(std::to_string(i), a.back());
        a.pop_back();
    }
}

}  // namespace alg::test
