#include <gtest/gtest.h>
#include "vector.hpp"

#include <iostream>
#include <list>

namespace alg::test {

TEST(Vector, Emplace) {
    Vector<std::string> v;
    std::string str = "ABC";
    v.emplace(v.begin(), str.begin(), str.end());
    EXPECT_EQ(1, v.size());
    EXPECT_EQ(*v.begin(), str);
}
TEST(Vector, Insert) {
    Vector<std::string> v;
    v.insert(v.begin(), "F");
    v.insert(v.begin(), {"B", "C", "D", "E"});
    v.insert(v.begin(), 3, "A");
    Vector<std::string> expected = {"A", "A", "A", "B", "C", "D", "E", "F"};
    EXPECT_EQ(expected, v);
}
TEST(Vector, Erase) {
    Vector<std::string> v({"A", "B", "C", "D", "E", "F"});
    v.erase(v.begin());
    v.erase(v.begin() + 1, v.end() - 1);
    Vector<std::string> expected({"B", "F"});
    EXPECT_EQ(expected, v);
}
}  // namespace alg::test
