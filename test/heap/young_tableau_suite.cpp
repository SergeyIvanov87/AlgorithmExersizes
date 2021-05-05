#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
#include <type_traits>

#include "heap/young_tableau/young_tableau.hpp"

#include <gtest/gtest.h>

#include "utils/printer.hpp"

template<class ...Lambdas>
struct LambdaTracer : public Lambdas...
{
    LambdaTracer(Lambdas ...l) :
        Lambdas(l)...
    {}

    using Lambdas::operator () ...;
};


namespace heap
{

using in_array_t = std::vector<int>;
using row_count_t =  int;
using column_count_t = int;
using expected_array_t = in_array_t;

template<int M, int N>
using YoungTBuildParams = std::tuple<in_array_t,
                                     std::integral_constant<int, M>,
                                     std::integral_constant<int, N>, expected_array_t>;

#define COMMA   ,


template<int M, int N>
class YoungTableauBuildFixture : public testing::TestWithParam<YoungTBuildParams<M, N>> {};

using YoungTableauBuildFixture_3x3 = YoungTableauBuildFixture<3, 3>;
using YoungTBuildParams_3x3 = YoungTBuildParams<3, 3>;



auto empty_printer = [] ()
{
};

TEST_P(YoungTableauBuildFixture_3x3, Build_3x3)
{
    in_array_t array = std::get<0>(GetParam());
    //row_count_t M = std::get<1>(GetParam());
    //column_count_t N = std::get<2>(GetParam());
    const expected_array_t& expected = std::get<3>(GetParam());

    LambdaTracer tracer(empty_printer);
    young_tableau<int, 3, 3> yt;

    for(auto v : array)
    {
        yt.insert(v, tracer);
    }
    ASSERT_EQ(yt.get_data(), expected) << "YT array is not equal with expected";
}

INSTANTIATE_TEST_SUITE_P(HeapSortGroup, YoungTableauBuildFixture_3x3,
                         testing::Values(
                            YoungTBuildParams_3x3 {
                                            {1, 2, 3, 4, 5, 6, 7, 8, 9},
                                            {}, {},
                                            {9, 8, 7, 6, 5, 4, 3, 2, 1}},
                             YoungTBuildParams_3x3 {
                                            {9, 16, 3, 2, 4, 8},
                                            {}, {},
                                            {16, 9, 8, 4, 3, 2, std::numeric_limits<int>::min(), std::numeric_limits<int>::min(), std::numeric_limits<int>::min()}}
                         ));

#undef COMMA
}
