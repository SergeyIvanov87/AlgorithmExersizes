#include <iostream>

#include <gtest/gtest.h>

#include "utils/tracer.hpp"

#include "sort/quicksort/quicksort.hpp"
namespace sort
{

using array_t = std::vector<int>;
using p_index_t = size_t;
using r_index_t = size_t;

using q_index = size_t;
using expected_tuple_t = std::tuple<q_index, array_t>;

using PartitionTestParams = std::tuple<array_t, p_index_t, r_index_t, expected_tuple_t>;

auto printer_a = [](const array_t& A, size_t pivot_index, std::initializer_list<size_t> highlight_indices = {})
{
    if (A.size() > pivot_index)
    {
        std::cout << "\npivot " << ::utils::bold_on << "A[" << pivot_index << "]: " << A[pivot_index] << ::utils::bold_off << ", A.size: " << A.size() << std::endl;
    }
    else
    {
        return;
    }

    ::utils::print_array_with_indices(std::cout, A, highlight_indices);
};

template<class IndicesContainer>
auto printer_range = [](const array_t& A, size_t pivot_index, size_t cur_index, IndicesContainer highlight_indices = {})
{
    if (A.size() > pivot_index)
    {
        std::cout << "\npivot " << ::utils::bold_on << "A[" << pivot_index << "]: " << A[pivot_index] << ::utils::bold_off << ", A.size: " << A.size() << std::endl;
    }
    else
    {
        return;
    }

    if(cur_index  < A.size())
    {
        auto it = highlight_indices.find(cur_index);
        if (it == highlight_indices.end())
        {
            highlight_indices[cur_index] = {"<", ">"};
        }
    }
    ::utils::print_array_with_indices(std::cout, A, highlight_indices);
};


class PartitionFixture : public testing::TestWithParam<PartitionTestParams> {};
TEST_P(PartitionFixture, empty)
{
    array_t array = std::get<0>(GetParam());
    p_index_t p = std::get<1>(GetParam());
    r_index_t r = std::get<2>(GetParam());

    expected_tuple_t expected = std::get<3>(GetParam());

    ::utils::LambdaTracer tracer {printer_range<::utils::highlighter_table_t>, printer_a};
    size_t q_got = details::partition(array, p, r, tracer);
    ASSERT_EQ(q_got, std::get<0>(expected)) << "Partition index 'q' is unexpected";
}

INSTANTIATE_TEST_SUITE_P(PartitionGroup, PartitionFixture,
                         testing::Values(
                            PartitionTestParams{
                                            {1,2,3,4,5,6,7}, 0, 6,
                                                {6, {1,2,3,4,5,6,7}}},
                            PartitionTestParams{
                                            {7,6,5,4,3,2,1}, 0, 6,
                                                {0, {1,6,5,4,3,2,7}}},
                            PartitionTestParams{
                                            {5,8,10,4,3,11,7}, 0, 6,
                                                {3, {5,4,3,7,8,10,11}}}
                         ));


}
