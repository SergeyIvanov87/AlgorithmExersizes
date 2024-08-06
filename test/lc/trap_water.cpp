#include <iostream>

#include <gtest/gtest.h>
#include "lc/trapping_water.hpp"

namespace lc
{

using array_t = std::vector<int>;
using expected_value_t = int;

using TrappingWaterGapParams = std::tuple<array_t, expected_value_t>;

class TrappingWaterFixture : public testing::TestWithParam<TrappingWaterGapParams> {};
TEST_P(TrappingWaterFixture, brute_force)
{
    array_t array = std::get<0>(GetParam());
    expected_value_t expected = std::get<1>(GetParam());

    size_t got = trap_water::trap_brute_force(array);
    ASSERT_EQ(got, expected);
}

TEST_P(TrappingWaterFixture, dyna)
{
    array_t array = std::get<0>(GetParam());
    expected_value_t expected = std::get<1>(GetParam());

    size_t got = trap_water::trap_linear_add_memory(array);
    ASSERT_EQ(got, expected);
}

INSTANTIATE_TEST_SUITE_P(TrappingWaterGroup, TrappingWaterFixture,
                         testing::Values(
                            TrappingWaterGapParams{ {0,1,0,2,1,0,1,3,2,1,2,1}, 6},
                            TrappingWaterGapParams{ {4,2,0,3,2,5}, 9}
                         ));
} // lc
