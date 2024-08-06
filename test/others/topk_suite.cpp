#include <iostream>

#include <gtest/gtest.h>

#include "utils/tracer.hpp"
#include "others/topk.hpp"

namespace others {
namespace topk
{
using element_t = int;
using element_array_t = std::vector<element_t>;
using index_t = size_t;
using index_array_t = std::list<index_t>;
using topk_t = size_t;

using expected_tuple_t = std::tuple<index_array_t>;
using TopKTestParams = std::tuple<element_array_t, topk_t, expected_tuple_t>;


class TopKFixture : public testing::TestWithParam<TopKTestParams> {};

TEST_P(TopKFixture, find_max_topk)
{
    const element_array_t& array = std::get<0>(GetParam());
    topk_t amount = std::get<1>(GetParam());
    const expected_tuple_t &expected = std::get<2>(GetParam());

    StdoutTracer stdout_tracer;
    auto got = find_topk_indices(array, amount, stdout_tracer);
    ASSERT_EQ(got, std::get<0>(expected)) << "TopK indices doesn't not equal to expected";
}
INSTANTIATE_TEST_SUITE_P(TopkGroup, TopKFixture,
                         testing::Values(
                            TopKTestParams{
                                            {1,2,3,-4,6,-9,11}, 0,
                                            {}},
                            TopKTestParams{
                                            {1,1,1,1,1,1,1}, 4,
                                            {{0,1,2,3}}},
                            TopKTestParams{
                                            {1,1,1,1,1,1,1}, 4000,
                                            {{0,1,2,3,4,5,6}}},
                            TopKTestParams{
                                            {1,2,3,4,5,6,7}, 7,
                                            {{6,5,4,3,2,1,0}}},
                            TopKTestParams{
                                            {1,2,3,4,5,6,7}, 4,
                                            {{6,5,4,3}}},
                            TopKTestParams{
                                            {7,6,5,4,3,2,1}, 7,
                                            {{0,1,2,3,4,5,6}}},
                            TopKTestParams{
                                            {7,6,5,4,3,2,1}, 4,
                                            {{0,1,2,3}}},
                            TopKTestParams{
                                            {1,7,3,5,4,6,2}, 4,
                                            {{1,5,3,4}}}
                         ));


}
}
