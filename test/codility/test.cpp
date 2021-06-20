#include <iostream>

#include <gtest/gtest.h>
#include "codility/digit_gap.hpp"

namespace codility
{

using array_t = std::vector<int>;
using expected_value_t = int;

using DigitGapParams = std::tuple<array_t, expected_value_t>;

class DigitGapFixture : public testing::TestWithParam<DigitGapParams> {};
TEST_P(DigitGapFixture, empty)
{
    array_t array = std::get<0>(GetParam());
    expected_value_t expected = std::get<1>(GetParam());

    size_t got = digit_gap(array);
    ASSERT_EQ(got, expected);
}

INSTANTIATE_TEST_SUITE_P(DigitGapGroup, DigitGapFixture,
                         testing::Values(
                            DigitGapParams{ {1}, 2},
                            DigitGapParams{ {3}, 1},
                            DigitGapParams{ {-2, -1, -3}, 1},
                            DigitGapParams{ {-20, -10, -30}, 1},
                            DigitGapParams{ {1, 1, 2, 3, 4, 6, 7, 9}, 5},
                            DigitGapParams{ {10, 10, 20, 30, 40, 60, 70, 90}, 1},
                            DigitGapParams{ {-30, -1, -2, -999, 1, 1, 2, 3, 4, 6, 7, 9}, 5},
                            DigitGapParams{ {-30, -1, -2, -999, 2, 3, 4, 6, 7, 9}, 1},
                            DigitGapParams{ {-30, -1, -2, -999, 0, 1, 2, 3, 4, 6, 7, 9}, 5}
                         ));
}
