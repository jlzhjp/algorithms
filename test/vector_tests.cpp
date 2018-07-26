#include <iostream>
#include <list>

#include <gtest/gtest.h>

#include "../src/vector.hpp"

namespace alg::test {

TEST(Vector, Constructor) {
    Vector<std::string> v1;
    EXPECT_EQ(0, v1.size());
    Vector<std::string> v2 = {"A", "B", "C"};
    EXPECT_EQ(3, v2.size());
    Vector<std::string> v3(10);
    EXPECT_EQ(10, v3.capacity());
    EXPECT_EQ(0, v3.size());
}
TEST(Vector, CopyConstructor) {
    Vector<std::string> v1 = {"A", "B", "C"};
    Vector<std::string> v2 = v1;
    EXPECT_EQ(v1, v2);
}
TEST(Vector, MoevConstructor) {
    Vector<std::string> v1 = {"A", "B", "C"};
    Vector<std::string> v2 = std::move(v1);
    Vector<std::string> expected({"A", "B", "C"});
    EXPECT_EQ(expected, v2);
}
TEST(Vector, CopyAssignment) {
    Vector<std::string> v1 = {"A", "B", "C"};
    Vector<std::string> v2 = {"D", "E"};
    v2 = v1;
    EXPECT_EQ(v1, v2);
}
TEST(Vector, MoveAssignment) {
    Vector<std::string> v1 = {"A", "B", "C"};
    Vector<std::string> v2 = {"D", "E"};
    v2 = std::move(v1);
    Vector<std::string> expected = {"A", "B", "C"};
    EXPECT_EQ(expected, v2);
}
TEST(Vector, Assign) {
    Vector<std::string> v;

    v.assign(30, "A");
    EXPECT_EQ(v.size(), 30);
    for (auto val : v) EXPECT_EQ("A", val);

    std::list<std::string> lst = { "A", "B", "C"};
    v.assign(lst.begin(), lst.end());
    EXPECT_TRUE(std::equal(v.begin(), v.end(), lst.begin(), lst.end()));


    std::initializer_list<std::string> ilst = { "A", "B", "C"};
    v.assign(ilst.begin(), ilst.end());
    EXPECT_TRUE(std::equal(v.begin(), v.end(), ilst.begin(), ilst.end()));
}
TEST(Vector, Emplace) {
    Vector<std::string> v;
    std::string str = "ABC";
    v.emplace(v.begin(), str.begin(), str.end());
    EXPECT_EQ(1, v.size());
    EXPECT_EQ(*v.begin(), str);
}
TEST(Vector, EmplaceBack) {
    Vector<std::string> v;
    std::string str = "ABC";
    v.emplace_back(str.begin(), str.end());
    v.emplace_back(str.begin(), str.end());
    EXPECT_EQ("ABC", v[0]);
    EXPECT_EQ("ABC", v[1]);
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
TEST(Vector, Clear) {
    Vector<std::string> v({"A", "B", "C"});
    v.clear();
    EXPECT_EQ(0, v.size());
}
TEST(Vector, Resize) {
    Vector<std::string> v({"A", "B", "C"});
    v.resize(5, "D");
    Vector<std::string> expected = {"A", "B", "C", "D", "D"};
    EXPECT_EQ(expected, v);
    v.resize(2);
    expected = {"A", "B"};
    EXPECT_EQ(expected, v);
}
TEST(Vector, Reserve) {
    Vector<std::string> v = { "A", "B" };
    auto cap = v.capacity();
    v.reserve(0);
    EXPECT_EQ(cap, v.capacity());
    v.reserve(100);
    EXPECT_EQ(100, v.capacity());
}
TEST(Vector, Swap) {
    Vector<std::string> v1 =  { "A", "B", "C" };
    Vector<std::string> v2 =  { "D", "E" };
    v1.swap(v2);
    Vector<std::string> v1_expected = { "D", "E" };
    Vector<std::string> v2_expected =  { "A", "B", "C" };
    EXPECT_EQ(v1_expected, v1);
    EXPECT_EQ(v2_expected, v2);
}
TEST(Vector, Stress) {
    Vector<std::string> v(0);    
    for (int i = 0; i <= 10000; ++i) {
        v.push_back(std::to_string(i));
    }
    for (int i = 10000; i >= 0; --i) {
        EXPECT_EQ(std::to_string(i), v.back());
        v.pop_back();
    }
}

}  // namespace alg::test