#ifndef LINEAR_COUNTING_SORT_HPP
#define LINEAR_COUNTING_SORT_HPP
#include "utils/printer.hpp"

namespace sort
{

#define  TRACE_CUR_INDEX(index) \
do { \
    tracer.cur_element(A, C, index); \
} while(false);


#define  TRACE_INPUT_ELEMENTS_COUNT_IN_ABC_SET \
do { \
    tracer(A, C, alphabet_size); \
} while(false);


#define  TRACE_PREV_ABC_ELEMENT_NOT_EXCEEDED_NEXT_ABC_ELEMENT_COUNT_IN_ABC_SET \
do { \
    tracer(A, C); \
} while(false);


#define  TRACE_INSERTION_ELEMENT_IN_ORDER(index) \
do { \
    tracer.place_element(A, B, C, index); \
} while(false);


template<class T, class U, class Tracer>
void countingsort(std::vector<T> &A, std::vector<T>& B, U alphabet_size, Tracer tracer, U exp = 1)
{
    std::vector<T> C(alphabet_size, T(0));
    for(size_t i = 0; i < A.size(); i ++)
    {
        T elem_value = (A[i] / exp) % alphabet_size; // % by `alphabet_size` is for radix_sort
        C[elem_value] = C[elem_value] + 1;

        TRACE_CUR_INDEX(i);
    }

    TRACE_INPUT_ELEMENTS_COUNT_IN_ABC_SET;

    for(size_t i = 1; i < alphabet_size; i ++)
    {
        C[i] = C[i] + C[i - 1];
    }

    TRACE_PREV_ABC_ELEMENT_NOT_EXCEEDED_NEXT_ABC_ELEMENT_COUNT_IN_ABC_SET

    B.resize(A.size());
    for(size_t i = A.size(); i != 0; i --)
    {
        auto j = i - 1;

        T elem_value = (A[j] / exp) % alphabet_size; // % by `alphabet_size` is for radix_sort
        B[C[elem_value] - 1] = A[j];   //initial value (not by module `alphabet_size`) is for radix sort
        C[elem_value] = C[elem_value] - 1;

        TRACE_INSERTION_ELEMENT_IN_ORDER(j)
    }
}

}

#endif //LINEAR_COUNTING_SORT_HPP
