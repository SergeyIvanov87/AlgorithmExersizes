#ifndef QUICK_SORT_HPP
#define QUICK_SORT_HPP
#include "utils/printer.hpp"

namespace sort
{

namespace details
{
    template<class T, class Tracer>
    size_t partition(std::vector<T>& A, size_t p, size_t r, Tracer tracer);
}
template<class T, class Tracer>
void quicksort(std::vector<T> &A, size_t p, size_t r, Tracer tracer)
{
    if(p < r)
    {
        size_t q = details::partition(A, p, r, tracer);
        quicksort(A, p, q - 1, tracer);
        quicksort(A, q, r, tracer);
    }
}





namespace details
{

#define  TRACE_CUR_INDEX(index) \
do { \
    tracer(A, r, index, greater_than_pivot_indices); \
} while(false);

#define TO_TRACE__ELSE_REMEMBER_GREATER_ELEMENT(index) \
else \
{ \
    do { \
        greater_than_pivot_indices[j] = {"(", ")"}; \
    } while(false); \
}

#define TO_TRACE__REMEMBER_SWAP_FIRST_GREATER_W_LAST_LESSER(greater_index, lesser_index) \
do { \
    if(greater_index != lesser_index) { \
        greater_than_pivot_indices.erase(greater_index); \
        greater_than_pivot_indices[lesser_index] = {"(", ")"}; \
    } \
} while(false);

#define TRACE_FINAL_PARTITION() \
do { \
    greater_than_pivot_indices.erase(i); \
    greater_than_pivot_indices[r] = {"(", ")"}; \
    tracer(A, i, inf_index, greater_than_pivot_indices); \
} while(false);




template<class T, class Tracer>
size_t partition(std::vector<T>& A, size_t p, size_t r, Tracer tracer)
{
    T pivot = A[r];
    size_t i = p;

    utils::highlighter_table_t greater_than_pivot_indices;
    utils::highlighter_table_t cur_index;
    size_t inf_index = std::numeric_limits<size_t>::max();
    for(size_t j = p; j < r; j ++)
    {
        TRACE_CUR_INDEX(j);
        if(A[j] < pivot)
        {
            std::swap(A[j], A[i]);
            i++;

            TO_TRACE__REMEMBER_SWAP_FIRST_GREATER_W_LAST_LESSER(i - 1, j);
        }
        TO_TRACE__ELSE_REMEMBER_GREATER_ELEMENT(J)
    }

    {
        tracer(A, r, inf_index,  greater_than_pivot_indices);
    }

    std::swap(A[r], A[i]);
    TRACE_FINAL_PARTITION();
    return i;
}
}
}
#endif //QUICK_SORT_HPP
