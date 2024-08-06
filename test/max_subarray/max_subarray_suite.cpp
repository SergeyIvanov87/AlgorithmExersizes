#include <iostream>

#include <gtest/gtest.h>

#include "utils/tracer.hpp"
#include "max_subarray/find_max_subarray.hpp"

namespace max_subarray
{
using array_t = std::vector<int>;
using index_t = size_t;
using left_index_t = index_t;
using right_index_t = index_t;
using expected_tuple_t = std::tuple<left_index_t, right_index_t, typename array_t::value_type>;
using MaxCrossArrayTestParams = std::tuple<array_t, left_index_t, index_t, right_index_t, expected_tuple_t>;
using MaxSubarrayTestParams = std::tuple<array_t, left_index_t, right_index_t, expected_tuple_t>;

class MaxCrossArrayFixture : public testing::TestWithParam<MaxCrossArrayTestParams> {};
class MaxSubArrayFixture : public testing::TestWithParam<MaxSubarrayTestParams> {};

TEST_P(MaxCrossArrayFixture, find_cross_array)
{
    const array_t& array = std::get<0>(GetParam());
    index_t low = std::get<1>(GetParam());
    index_t mid = std::get<2>(GetParam());
    index_t high = std::get<3>(GetParam());
    expected_tuple_t expected = std::get<4>(GetParam());

    StdoutTracer stdout_tracer;
    auto got = find_max_crossing_subarray(array, low, mid, high, stdout_tracer);
    ASSERT_EQ(got, expected) << "Max subarray is not equal with expected";
}
INSTANTIATE_TEST_SUITE_P(MaxCrossArrayGroup, MaxCrossArrayFixture,
                         testing::Values(
                            MaxCrossArrayTestParams{
                                            {1,2,3,-4,6,-9,11}, 0, 6, 6,
                                                {6, 6, 11}},
                            MaxCrossArrayTestParams{
                                            {1,2,3,-4,6,-9,11}, 0, 0, 6,
                                                {0, 6, 10}},
                            MaxCrossArrayTestParams{
                                            {1,2,3,-4,6,-9,11}, 0, 3, 6,
                                                {0, 6, 10}},
                            MaxCrossArrayTestParams{
                                            {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7}, 0, 7, 15,
                                               {7, 10, 43}}
                         ));


TEST_P(MaxSubArrayFixture, find_max_array)
{
    const array_t& array = std::get<0>(GetParam());
    index_t low = std::get<1>(GetParam());
    index_t high = std::get<2>(GetParam());
    expected_tuple_t expected = std::get<3>(GetParam());

    StdoutTracer stdout_tracer;
    auto got = find_max_subarray(array, low, high, stdout_tracer);
    ASSERT_EQ(got, expected) << "Max subarray is not equal with expected";
}


INSTANTIATE_TEST_SUITE_P(MaxSubArrayGroup, MaxSubArrayFixture,
                         testing::Values(
                            MaxSubarrayTestParams{
                                            {1,2,3,-4,6,-9,11}, 0, 6,
                                                {6, 6, 11}},
                            MaxSubarrayTestParams{
                                            {13,-3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7}, 0, 15,
                                                {7, 10, 43}}
                         ));

namespace linear2
{
class MaxSubArrayLinearFixture : public testing::TestWithParam<MaxSubarrayTestParams> {};

TEST_P(MaxSubArrayLinearFixture, find_max_array_linear)
{
    const array_t& array = std::get<0>(GetParam());
    index_t low = std::get<1>(GetParam());
    index_t high = std::get<2>(GetParam());
    expected_tuple_t expected = std::get<3>(GetParam());

    StdoutTracer stdout_tracer;
    auto got = find_max_subarray_linear(array, low, high, stdout_tracer);
    ASSERT_EQ(got, expected) << "Max subarray is not equal with expected";
}


INSTANTIATE_TEST_SUITE_P(MaxSubArrayLinearGroup, MaxSubArrayLinearFixture,
                         testing::Values(
                            MaxSubarrayTestParams{
                                            {1,2,3,-4,6,-9,11}, 0, 6,
                                                {6, 6, 11}},
                            MaxSubarrayTestParams{
                                            {13,-3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7}, 0, 15,
                                                {7, 10, 43}},
                            MaxSubarrayTestParams{
                                            {1, 2, -9, 1, 2, 3}, 0 , 5,
                                                {3, 5, 6}}
                         ));

}
}
