#include <iostream>

#include <gtest/gtest.h>

#include "utils/tracer.hpp"
#include "sort/linear_complexity/counting_sort.hpp"

namespace sort
{

using array_t = std::vector<int>;
using alphabet_t = size_t;
using expected_array_t = array_t;

using CountinSortTestParams = std::tuple<array_t, alphabet_t, expected_array_t>;

class CountingSortFixture : public testing::TestWithParam<CountinSortTestParams> {};



namespace printer
{
    struct Tracer
    {
        template<class T>
        void cur_element(const std::vector<T>& A, const std::vector<T> &C, size_t cur_index)
        {
            std::cout << "Calculate elements count in ABC set" << std::endl;
            ::utils::highlighter_table_t highlight_indices;
            highlight_indices[cur_index] = {"<", ">"};
            ::utils::print_array_with_indices(std::cout, A, highlight_indices);
            std::cout << "Occurence count of A[" << cur_index << "]=" << A[cur_index] << " in ABC set:" << std::endl;
            ::utils::print_array_with_indices(std::cout, C, { static_cast<size_t>(A[cur_index]) });
            std::cout << std::endl;
        }

        template<class T>
        void operator() (const std::vector<T>& A, const std::vector<T> &C, size_t abc_size)
        {
            std::cout << "Non-sorted array:" << std::endl;
            ::utils::print_array_with_indices(std::cout, A, { });
            std::cout << "Elements occurences in ABC set FINAL: " << std::endl;
            ::utils::print_array_with_indices(std::cout, C, { });
        }

        template<class T>
        void operator() (const std::vector<T>& A, const std::vector<T> &C)
        {
            std::cout << "Elements count \"not more then next\" distribution in increasing orders: " << std::endl;
            ::utils::print_array_with_indices(std::cout, C, { });
        }

        template<class T>
        void place_element(const std::vector<T>& A, const std::vector<T> &B, const std::vector<T> &C, size_t index)
        {
            std::cout << "" << std::endl;
        }
    };
}


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
