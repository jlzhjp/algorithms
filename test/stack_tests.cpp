#include <gtest/gtest.h>

#include "../src/stack.hpp"

namespace alg::test {

TEST(Stack, Constructor_Overload1) {
    Stack<int> s;

    EXPECT_EQ(0, s.size());
    EXPECT_EQ(16, s.capacity());
}
TEST(Stack, Constructor_Overload2) {
    Stack<std::string> s(5);

    EXPECT_EQ(0, s.size());
    EXPECT_EQ(5, s.capacity());
}
TEST(Stack, Constructor_Overload3) {
    Stack<std::string> s = {"Hello", "World"};

    EXPECT_EQ(2, s.size());
    EXPECT_EQ(18, s.capacity());
}
TEST(Stack, CopyConstructor) {
    Stack<std::string> s1 = {"Hello", "World"};
    Stack<std::string> s2 = s1;
    EXPECT_EQ(s1.size(), s2.size());
    EXPECT_EQ(s1.capacity(), s2.capacity());
    EXPECT_EQ(s1.pop(), s2.pop());
    EXPECT_EQ(s1.pop(), s2.pop());
}
TEST(Stack, MoveConstructor) {
    using size_type = Stack<std::string>::size_type;
    Stack<std::string> s1 = {
        "Hello",
        "World",
    };
    size_type size = s1.size();
    size_type cap = s1.capacity();
    Stack<std::string> s2 = std::move(s1);
    EXPECT_EQ(size, s2.size());
    EXPECT_EQ(cap, s2.capacity());
    EXPECT_EQ("World", s2.pop());
    EXPECT_EQ("Hello", s2.pop());
}
TEST(Stack, CopyAssignment) {
    Stack<std::string> s1 = {
        "Hello",
        "World",
    };
    Stack<std::string> s2 = {
        "!",
    };
    s2 = s1;
    EXPECT_EQ(s1.size(), s2.size());
    EXPECT_EQ(s1.capacity(), s2.capacity());
    EXPECT_EQ(s1.pop(), s2.pop());
    EXPECT_EQ(s1.pop(), s2.pop());
}
TEST(Stack, MoveAssignment) {
    using size_type = Stack<std::string>::size_type;
    Stack<std::string> s1 = {"Hello", "World"};
    size_type size = s1.size();
    size_type cap = s1.capacity();
    Stack<std::string> s2 = {"!"};
    s2 = std::move(s1);
    EXPECT_EQ(size, s2.size());
    EXPECT_EQ(cap, s2.capacity());
    EXPECT_EQ("World", s2.pop());
    EXPECT_EQ("Hello", s2.pop());
}
TEST(Stack, Push) {
    Stack<std::string> s = {"Hello", "World"};
    s.push("!");
    EXPECT_EQ("!", s.top());
}
TEST(Stack, Pop) {
    Stack<std::string> s = {"Hello", "World"};
    EXPECT_EQ("World", s.pop());
    EXPECT_EQ("Hello", s.pop());
}
TEST(Stack, Empty) {
    Stack<std::string> s;
    EXPECT_TRUE(s.empty());
    s.push("Hello");
    EXPECT_FALSE(s.empty());
}
TEST(Stack, Swap) {
    Stack<std::string> s1 = {"Hello", "World"};
    Stack<std::string> s2 = {"Hello", "World", "!"};
    std::swap(s1, s2);

    EXPECT_EQ(3, s1.size());
    EXPECT_EQ(2, s2.size());
    EXPECT_EQ(s1.pop(), "!");
    EXPECT_EQ("World", s1.pop());
    EXPECT_EQ("Hello", s1.pop());
    EXPECT_EQ("World", s2.pop());
    EXPECT_EQ("Hello", s2.pop());
}

TEST(Stack, Stress) {
    Stack<std::string> s;
    for (int i = 0; i <= 10000; ++i) {
        s.push(std::to_string(i));
    }
    for (int i = 10000; i >= 0; --i) {
        EXPECT_EQ(std::to_string(i), s.pop());
    }
}
}  // namespace alg::test
