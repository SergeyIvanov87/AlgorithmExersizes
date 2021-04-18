#ifndef HEAP_PROPERTIES_HPP
#define HEAP_PROPERTIES_HPP

#include <cassert>
#include <vector>

namespace heap
{
template<class T>
using array_t = std::vector<T>;
using index_t = size_t;

namespace utils
{
    index_t parent(index_t i)
    {
         return i / 2;
    }

    index_t left(index_t i)
    {
        return i * 2 + 1;
    }

    index_t right(index_t i)
    {
        return i * 2 + 2;
    }
}

template<class T, class Trace, class Comparison = std::less<T>>
void max_heapify(array_t<T>& A, index_t i, Trace tracer, Comparison cmp = Comparison())
{
    using namespace utils;
    assert(i < A.size() && "max_heapity: index is greater than expected");

    tracer(A, i);

    index_t l = left(i);
    index_t r = right(i);

    index_t largest = i;

    tracer(A, i, l);
    if(l < A.size() and cmp(A[i], A[l])) // A[i] < A[Left] breake non-increasing heap-property
    {
        largest = l;
        tracer(A[largest], largest);
    }

    tracer(A, largest, r);
    if(r < A.size() && cmp(A[largest], A[r])) // or A[i] < A[right] and breake non-increasing heap properpy
    {
        largest = r;
        tracer(A[largest], largest);
    }


    if(largest != i)
    {
        //make exchange
        std::swap(A[i], A[largest]);

        max_heapify(A, largest, tracer, cmp);
    }
}

}
#endif //HEAP_PROPERTIES_HPP
