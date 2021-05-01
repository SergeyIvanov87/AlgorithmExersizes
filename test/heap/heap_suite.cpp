#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>

#include "heap/heap_operations.hpp"
#include "heap/priority_queue.hpp"

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
template<class T>
auto printer_a = [](const array_t<T>& A, index_t i, std::initializer_list<size_t> highlight_indices = {})
{
    if (A.size() > i)
    {
        std::cout << "\nmax-heapify for " << ::utils::bold_on << "A[" << i << "]: " << A[i] << ::utils::bold_off << ", A.size: " << A.size() << std::endl;
    }
    else
    {
        std::cout << "\nmax-heapify for A[out-of-range: " << i << " ], A.size: " << A.size() << std::endl;
    }

    ::utils::print_array_with_indices(std::cout, A, highlight_indices);
};

template<class T>
auto printer_cmp = [](const array_t<T>& A, index_t i, index_t j, index_t winner)
{
    if(A.size() > i)
    {
        std::cout << std::endl << ::utils::bold_on <<"       ["<< A[i] << "]" << ::utils::bold_off;
    }
    else
    {
        std::cout << "A[`i` out-of-range: " << i << " ]";
    }

    if(A.size() > j)
    {
        std::cout << " ? " << ::utils::bold_on << "(" << A[j] << ")" << ::utils::bold_off;
    }
    else
    {
        std::cout << " and A[`j` out-of-range: " << j << " ]" << std::endl;
    }

    if(A.size() > winner)
    {
        std::cout << " -> " << ::utils::bold_on << "[" << A[winner] << "]" << ::utils::bold_off << std::endl;
    }
    std::cout << std::endl;
};

template<class T>
auto printer_largest = [](T val, index_t i)
{
    std::cout << "Result A[" << i << "]: " << val << std::endl;
};

template<class T>
auto printer_exchanged = [](const array_t<T>& A, std::initializer_list<size_t> highlight_indices = {})
{
    ::utils::print_array_with_indices(std::cout, A, highlight_indices);
};

TEST(HeapSuite, Heapify)
{
    LambdaTracer tracer(printer_a<int>, printer_cmp<int>, printer_largest<int>, printer_exchanged<int>);

    array_t<int> A{10, 2, 6, 4, 5, 3, 4};
    array_t<int> AA = A;

    heapify(A, A.size(), 1, tracer);
    heapify_no_recursion(AA, AA.size(), 1, tracer);

    array_t<int> R{10, 5, 6, 4, 2, 3, 4};
    ASSERT_EQ(A, R);
    ASSERT_EQ(A, AA);

}

TEST(HeapSuite, BuildHeap)
{
    LambdaTracer tracer(printer_a<int>, printer_cmp<int>, printer_largest<int>, printer_exchanged<int>);

    array_t<int> A{4, 1, 3, 2, 16, 9, 10, 14, 8, 7};

    build_heap(A, A.size(), tracer);

    array_t<int> R{16, 14, 10, 8, 7, 9, 3, 2, 4, 1};
    ASSERT_EQ(A, R);
}




using in_array_t = array_t<int>;
using sorted_array_t = array_t<int>;
using HeapSortParams = std::tuple<in_array_t, sorted_array_t>;

class HeapSortFixture : public testing::TestWithParam<HeapSortParams> {};

TEST_P(HeapSortFixture, HeapSortTest)
{
    in_array_t array = std::get<0>(GetParam());
    const sorted_array_t& expected = std::get<1>(GetParam());

    LambdaTracer tracer(printer_a<int>, printer_cmp<int>, printer_largest<int>, printer_exchanged<int>);
    heap_sort(array, tracer);
    ASSERT_EQ(array, expected) << "Sorted array is not equal with expected";
}
INSTANTIATE_TEST_SUITE_P(HeapSortGroup, HeapSortFixture,
                         testing::Values(
                            HeapSortParams{
                                            {1, 2},
                                            {1, 2}},
                             HeapSortParams{
                                            {2, 1},
                                            {1, 2}},
                             HeapSortParams{
                                            {1},
                                            {1, }},
                             HeapSortParams{
                                            {2, 3, 1},
                                            {1, 2, 3}},
                             HeapSortParams{
                                            {16, 14, 10, 8, 7, 9, 3, 2, 4, 1},
                                            {1, 2, 3, 4, 7, 8, 9, 10, 14, 16}}
                         ));



template<class T>
auto printer_exchanged_info = [](const array_t<T>& A, std::initializer_list<size_t> highlight_indices = {}, std::string info = std::string())
{
    if (!info.empty())
    {
        std::cout << info << std::endl;
    }
    ::utils::print_array_with_indices(std::cout, A, highlight_indices);
};

TEST(PriorityQueueSuite, creation)
{
    LambdaTracer tracer(printer_a<int>, printer_cmp<int>, printer_largest<int>, printer_exchanged_info<int>);

    priority_queue<int> pq;
    pq.insert(100, tracer);
    std::cout << std::endl;

    pq.insert(200, tracer);
    std::cout << std::endl;

    pq.insert(10, tracer);
    std::cout << std::endl;

    pq.insert(20, tracer);
    std::cout << std::endl;

    pq.insert(900, tracer);
    std::cout << std::endl;

    ASSERT_EQ(pq.top(), 900);

    pq.extract_top(tracer);
    std::cout << std::endl;
    ASSERT_EQ(pq.top(), 200);

    pq.extract_top(tracer);
    std::cout << std::endl;
    ASSERT_EQ(pq.top(), 100);

    pq.extract_top(tracer);
    std::cout << std::endl;
    ASSERT_EQ(pq.top(), 20);

    pq.extract_top(tracer);
    std::cout << std::endl;
    ASSERT_EQ(pq.top(), 10);
}
}
