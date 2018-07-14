#include <gtest/gtest.h>

#include "../../src/math/evaluation.hpp"

namespace algo::math::test {

TEST(DijkstraExpressionEvaluation, Simple) {
    double result = algo::evaluate("( 1 + ( ( 2 + 3 ) * ( 4 * 5 ) ) )");
    EXPECT_EQ(1 + ((2 + 3) * (4 * 5)), result);
}

TEST(DijkstraExpressionEvaluation, Function) {
    double result = algo::evaluate("( ( 1 + sqrt ( 5.0 ) ) / 2.0 )");
    EXPECT_EQ((1 + std::sqrt(5.0)) / 2.0, result);
}
}
