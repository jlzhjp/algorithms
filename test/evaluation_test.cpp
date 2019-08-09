#include <gtest/gtest.h>
#include "evaluation.hpp"

#include <cmath>

namespace alg::test {

TEST(DijkstraExpressionEvaluation, Simple) {
    double result = alg::evaluate("( 1 + ( ( 2 + 3 ) * ( 4 * 5 ) ) )");
    EXPECT_EQ(1 + ((2 + 3) * (4 * 5)), result);
}

TEST(DijkstraExpressionEvaluation, Function) {
    double result = alg::evaluate("( ( 1 + sqrt ( 5.0 ) ) / 2.0 )");
    EXPECT_EQ((1 + std::sqrt(5.0)) / 2.0, result);
}

}  // namespace alg::test
