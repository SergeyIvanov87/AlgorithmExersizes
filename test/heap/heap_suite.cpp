#include <iostream>
#include <iterator>
#include <numeric>

#include "heap/heap_properties.hpp"
#include <gtest/gtest.h>

namespace heap
{

template<class ...Lambdas>
struct LambdaTracer : public Lambdas...
{
    LambdaTracer(Lambdas ...l) :
        Lambdas(l)...
    {}

    using Lambdas::operator () ...;
};

template<class T>
auto printer_a = [](const array_t<T>& A, index_t i)
{
    if (A.size() > i)
    {
        std::cout << "max-heapify for A[" << i << "]: " << A[i]  << ", A.size: " << A.size() << std::endl;
    }
    else
    {
        std::cout << "max-heapify for A[out-of-range: " << i << " ], A.size: " << A.size() << std::endl;
    }

    std::copy(A.begin(), A.end(), std::ostream_iterator<T>(std::cout, ", "));
    std::cout << std::endl;
};

template<class T>
auto printer_cmp = [](const array_t<T>& A, index_t i, index_t j)
{
    if(A.size() > i)
    {
        std::cout << "CMP A[" << i << "]: " << A[i];
    }
    else
    {
        std::cout << "A[`i` out-of-range: " << i << " ]";
    }

    if(A.size() > j)
    {
        std::cout << " and A[" << j << "]: " << A[j] << std::endl;
    }
    else
    {
        std::cout << " and A[`j` out-of-range: " << j << " ]" << std::endl;
    }
};

template<class T>
auto printer_largest = [](T val, index_t i)
{
    std::cout << "Result A[" << i << "]: " << val << std::endl;
};

TEST(HeapSuite, MaxHeapify)
{
    LambdaTracer tracer(printer_a<int>, printer_cmp<int>, printer_largest<int>);

    array_t<int> A{10, 2, 6, 4, 5, 3, 4};

    max_heapify(A, 1, tracer);
}
}
