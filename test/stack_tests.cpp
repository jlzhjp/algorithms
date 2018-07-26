#include <gtest/gtest.h>

#include "../src/stack.hpp"

namespace alg::test {

TEST(Stack, Constructor_Overload1) {
    Stack<int> s1;
    EXPECT_EQ(0, s1.size());
    Stack<std::string> s2(5);
    EXPECT_EQ(0, s2.size());
    EXPECT_EQ(5, s2.capacity());
    Stack<std::string> s3 = {"A", "B"};
    EXPECT_EQ(2, s3.size());
}
TEST(Stack, CopyConstructor) {
    Stack<std::string> s1 = {"A", "B"};
    Stack<std::string> s2 = s1;
    EXPECT_EQ(s1, s2);
}
TEST(Stack, MoveConstructor) {
    using size_type = Stack<std::string>::size_type;
    Stack<std::string> s1 = {"A", "B"};
    size_type size = s1.size();
    size_type cap = s1.capacity();
    Stack<std::string> s2 = std::move(s1);
    EXPECT_EQ(size, s2.size());
    EXPECT_EQ(cap, s2.capacity());
    EXPECT_EQ("B", s2.pop());
    EXPECT_EQ("A", s2.pop());
}
TEST(Stack, CopyAssignment) {
    Stack<std::string> s1 = {"A", "B"};
    Stack<std::string> s2 = {"C"};
    s2 = s1;
    EXPECT_EQ(s1, s2);
}
TEST(Stack, MoveAssignment) {
    using size_type = Stack<std::string>::size_type;
    Stack<std::string> s1 = {"A", "B"};
    size_type size = s1.size();
    size_type cap = s1.capacity();
    Stack<std::string> s2 = {"C"};
    s2 = std::move(s1);
    EXPECT_EQ(size, s2.size());
    EXPECT_EQ(cap, s2.capacity());
    EXPECT_EQ("B", s2.pop());
    EXPECT_EQ("A", s2.pop());
}
TEST(Stack, Push) {
    Stack<std::string> s = {"A", "B"};
    s.push("C");
    EXPECT_EQ("C", s.top());
}
TEST(Stack, Pop) {
    Stack<std::string> s = {"A", "B"};
    EXPECT_EQ("B", s.pop());
    EXPECT_EQ("A", s.pop());
}
TEST(Stack, Empty) {
    Stack<std::string> s;
    EXPECT_TRUE(s.empty());
    s.push("A");
    EXPECT_FALSE(s.empty());
}
TEST(Stack, Swap) {
    Stack<std::string> s1 = {"A", "B"};
    Stack<std::string> s2 = {"C", "D", "E"};
    std::swap(s1, s2);

    EXPECT_EQ(3, s1.size());
    EXPECT_EQ(2, s2.size());
    EXPECT_EQ("E", s1.pop());
    EXPECT_EQ("D", s1.pop());
    EXPECT_EQ("C", s1.pop());
    EXPECT_EQ("B", s2.pop());
    EXPECT_EQ("A", s2.pop());
}

TEST(Stack, Stress) {
    Stack<std::string> s(0);
    for (int i = 0; i <= 10000; ++i) {
        s.push(std::to_string(i));
    }
    for (int i = 10000; i >= 0; --i) {
        EXPECT_EQ(std::to_string(i), s.pop());
    }
}

}  // namespace alg::test
