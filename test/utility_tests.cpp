#include <gtest/gtest.h>
#include "../src/utility.hpp"

namespace alg::test {

TEST(Utility, Eq) {
    EXPECT_TRUE(alg::eq(0, 0));
    EXPECT_FALSE(alg::eq(0, 1));
}
TEST(Utility, Ne) {
    EXPECT_TRUE(alg::ne(0, 1));
    EXPECT_FALSE(alg::ne(0, 0));
}
TEST(Utility, Lt) {
    EXPECT_TRUE(alg::lt(0, 1));
    EXPECT_FALSE(alg::lt(0, 0));
    EXPECT_FALSE(alg::lt(1, 0));
}
TEST(Utility, Gt) {
    EXPECT_TRUE(alg::gt(1, 0));
    EXPECT_FALSE(alg::gt(0, 0));
    EXPECT_FALSE(alg::gt(0, 1));
}
TEST(Utility, Le) {
    EXPECT_TRUE(alg::le(0, 1));
    EXPECT_TRUE(alg::le(0, 0));
    EXPECT_FALSE(alg::le(1, 0));
}
TEST(Utility, Ge) {
    EXPECT_TRUE(alg::ge(1, 0));
    EXPECT_TRUE(alg::ge(0, 0));
    EXPECT_FALSE(alg::ge(0, 1));
}

}  // namespace alg::test