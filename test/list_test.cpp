#include <gtest/gtest.h>

#include "list.hpp"

namespace alg::test {
TEST(List, Constructor) {
    List<std::string> lst1;
    List<std::string> lst2(10, "A");
    for (auto &v : lst1) {
        EXPECT_EQ(v, "A");
    }
    List<std::string> lst3{"A", "B"};
    EXPECT_EQ(*lst3.begin(), "A");
    EXPECT_EQ(*++lst3.begin(), "B");
}
TEST(List, CopyConstructor) {
    List<std::string> lst1{"A", "B"};
    List<std::string> lst2 = lst1;

    EXPECT_EQ(*lst1.begin(), "A");
    EXPECT_EQ(*++lst1.begin(), "B");
    EXPECT_EQ(*lst2.begin(), "A");
    EXPECT_EQ(*++lst2.begin(), "B");
}
}  // namespace alg::test
