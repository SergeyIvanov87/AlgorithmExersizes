#include <iostream>

#include <gtest/gtest.h>

#include "utils/tracer.hpp"
#include "utils/printer.hpp"
#include "sort/mergesort/merge_sort.hpp"

namespace sort
{

using array_t = std::vector<int>;
using begin_index_t = size_t;
using end_index_t = size_t;
using expected_array_t = array_t;

using MergeSortTestParams = std::tuple<array_t, begin_index_t, end_index_t, expected_array_t>;

class MergeSortFixture : public testing::TestWithParam<MergeSortTestParams> {};

namespace printer
{
    struct MergeSortTracer
    {
        MergeSortTracer() = default;
        MergeSortTracer(const MergeSortTracer& src)
        {
            prefix = src.prefix + "\t";
        }

        void add_prefix(const std::string &p)
        {
            prefix = p + prefix;
        }
        template<class T>
        void print_array(const std::vector<T>& A, size_t begin, size_t end)
        {
            ::utils::highlighter_table_t highlight_indices;
            highlight_indices[begin] = {"<", ""};
            highlight_indices[end].first += "";
            highlight_indices[end].second += ">";

            ::utils::print_array_with_indices(std::cout, A, highlight_indices, prefix);
        }

        template<class T>
        void print_partitioned_array(const std::vector<T>& A, size_t begin, size_t q, size_t end)
        {
            ::utils::highlighter_table_t highlight_indices;
            highlight_indices[begin] = {"<", ""};
            highlight_indices[q].first += "[";
            highlight_indices[q].second +=  "]";
            highlight_indices[end].first += "";
            highlight_indices[end].second += ">";


            ::utils::print_array_with_indices(std::cout, A, highlight_indices, prefix);
        }
        std::string prefix;
    };
}

TEST_P(MergeSortFixture, test)
{
    array_t in_array = std::get<0>(GetParam());
    begin_index_t p = std::get<1>(GetParam());
    end_index_t r = std::get<2>(GetParam());
    expected_array_t expected = std::get<3>(GetParam());

    printer::MergeSortTracer tracer;
    merge_sort(in_array, p, r, tracer);
    ASSERT_EQ(in_array, expected) << "Unexpected sorted array";
}

INSTANTIATE_TEST_SUITE_P(MergeSortTest, MergeSortFixture,
                         testing::Values(
                            MergeSortTestParams{
                                            {1,2,3,4,5,6,7}, 0, 2,
                                            {1,2,3,4,5,6,7}},
                            MergeSortTestParams{
                                            {1111111,222222,33333,4444,555,66,7}, 0, 6,
                                            {7,66,555,4444,33333,222222,1111111}},
                            MergeSortTestParams{
                                            {67,56,45,34,23,12,11}, 0, 6,
                                            {11,12,23,34,45,56,67}},
                            MergeSortTestParams{
                                            {5,18,10,994,3,1100,17}, 0, 6,
                                            {3,5,10,17,18,994,1100}}
                         ));


}
