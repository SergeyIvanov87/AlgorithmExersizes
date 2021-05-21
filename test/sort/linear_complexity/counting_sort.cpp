#include <iostream>

#include <gtest/gtest.h>

#include "tracer_impl.hpp"
#include "sort/linear_complexity/counting_sort.hpp"

namespace sort
{

using array_t = std::vector<int>;
using alphabet_t = size_t;
using expected_array_t = array_t;

using CountinSortTestParams = std::tuple<array_t, alphabet_t, expected_array_t>;

class CountingSortFixture : public testing::TestWithParam<CountinSortTestParams> {};


TEST_P(CountingSortFixture, test)
{
    array_t in_array = std::get<0>(GetParam());
    alphabet_t abc_size = std::get<1>(GetParam());
    expected_array_t expected = std::get<2>(GetParam());

    printer::Tracer tracer;
    array_t out_array;
    countingsort(in_array, out_array, abc_size, tracer);
    ASSERT_EQ(out_array, expected) << "Unexpected sorted array";
}

INSTANTIATE_TEST_SUITE_P(CountingSortTest, CountingSortFixture,
                         testing::Values(
                            CountinSortTestParams{
                                            {1,2,3,4,5,6,7}, 8,
                                            {1,2,3,4,5,6,7}},
                            CountinSortTestParams{
                                            {7,6,5,4,3,2,1}, 8,
                                            {1,2,3,4,5,6,7}},
                            CountinSortTestParams{
                                            {5,8,10,4,3,11,7}, 16,
                                            {3,4,5,7,8,10,11}}
                         ));


}
