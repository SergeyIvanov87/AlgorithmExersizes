#include <iostream>

#include <gtest/gtest.h>

#include "tracer_impl.hpp"
#include "sort/linear_complexity/radix_sort.hpp"

namespace sort
{

using array_t = std::vector<int>;
using digits_max_t = size_t;
using expected_array_t = array_t;

using RadixSortTestParams = std::tuple<array_t, digits_max_t, expected_array_t>;

class RadixSortFixture : public testing::TestWithParam<RadixSortTestParams> {};

namespace printer
{
    struct RadixTracer : public Tracer
    {
        using Tracer::operator ();
        void operator() (size_t digit_num)
        {
            std::cout << "\nStart countingsort by: " << digit_num << " digit" << std::endl;
        }
    };
}
TEST_P(RadixSortFixture, test)
{
    array_t in_array = std::get<0>(GetParam());
    digits_max_t max_digits = std::get<1>(GetParam());
    expected_array_t expected = std::get<2>(GetParam());

    printer::RadixTracer tracer;
    radix_sort(in_array, max_digits, tracer);
    ASSERT_EQ(in_array, expected) << "Unexpected sorted array";
}

INSTANTIATE_TEST_SUITE_P(RadixSortTest, RadixSortFixture,
                         testing::Values(
                            RadixSortTestParams{
                                            {1,2,3,4,5,6,7}, 1,
                                            {1,2,3,4,5,6,7}},
                            RadixSortTestParams{
                                            {67,56,45,34,23,12,11}, 2,
                                            {11,12,23,34,45,56,67}},
                            RadixSortTestParams{
                                            {5,18,10,994,3,1100,17}, 4,
                                            {3,5,10,17,18,994,1100}}
                         ));


}
