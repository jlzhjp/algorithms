#include <gtest/gtest.h>
#include "union_find/quick_find.hpp"
#include "union_find/quick_union.hpp"

namespace alg::test {

template <typename UF>
struct UnionFindTest : public ::testing::Test {
    using UFImpl = UF;
};

using UFImpls = ::testing::Types<alg::QuickFind, alg::QuickUnion>;
TYPED_TEST_CASE(UnionFindTest, UFImpls);

TYPED_TEST(UnionFindTest, Normal) {
    typename TestFixture::UFImpl uf(10);
    uf.connect(9, 1);
    uf.connect(1, 3);
    uf.connect(3, 5);
    EXPECT_TRUE(uf.is_connected(9, 5));
    EXPECT_FALSE(uf.is_connected(9, 4));
}

}  // namespace alg::test
