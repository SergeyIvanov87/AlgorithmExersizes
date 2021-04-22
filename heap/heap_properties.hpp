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
void heapify(array_t<T>& A, index_t heap_size, index_t i, Trace tracer, Comparison cmp = Comparison())
{
    using namespace utils;
    assert(i < heap_size && "max_heapity: index is greater than expected");

    index_t l = left(i);
    index_t r = right(i);

    tracer(A, i, {i, l, r});
    index_t largest = i;

    if(l < heap_size and cmp(A[i], A[l])) // A[i] < A[Left] breake non-increasing heap-property
    {
        tracer(A, i, l, l);
        largest = l;
    }

    if(r < heap_size && cmp(A[largest], A[r])) // or A[i] < A[right] and breake non-increasing heap properpy
    {
        tracer(A, largest, r, r);
        largest = r;
    }


    if(largest != i)
    {
        //make exchange
        std::swap(A[i], A[largest]);
        tracer(A, {largest, i});

        heapify(A, heap_size, largest, tracer, cmp);
    }
}


template<class T, class Trace, class Comparison = std::less<T>>
void heapify_no_recursion(array_t<T>& A, index_t heap_size, index_t i, Trace tracer, Comparison cmp = Comparison())
{
    using namespace utils;
    assert(i < heap_size && "max_heapity: index is greater than expected");

    index_t largest = i;
    do
    {
        //make exchange
        std::swap(A[i], A[largest]);

        i = largest;



        index_t l = left(i);
        index_t r = right(i);
        tracer(A, i, {i, l, r});

        if(l < heap_size and cmp(A[i], A[l])) // A[i] < A[Left] breake non-increasing heap-property
        {
            largest = l;
            tracer(A, i, l, largest);
        }

        if(r < heap_size && cmp(A[largest], A[r])) // or A[i] < A[right] and breake non-increasing heap properpy
        {
            tracer(A, largest, r, r);
            largest = r;
        }
    }
    while(largest != i);
}


template<class T, class Trace, class Comparison = std::less<T>>
void build_heap(array_t<T>& A, index_t heap_size, Trace tracer, Comparison cmp = Comparison())
{
    for(index_t i = heap_size / 2; i != 0; i--)
    {

        heapify(A, heap_size, i - 1, tracer, cmp);
    }
}


template<class T, class Trace, class Comparison = std::less<T>>
void heap_sort(array_t<T>& A, Trace tracer, Comparison cmp = Comparison())
{
    build_heap(A, A.size(), tracer, cmp);
    for(size_t i = A.size(); i != 1; i-- )
    {
        std::swap(A[0], A[i - 1]);
        build_heap(A, i - 1, tracer, cmp);
    }
}
}

#endif //HEAP_PROPERTIES_HPP
