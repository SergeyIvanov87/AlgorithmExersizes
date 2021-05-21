#ifndef LINEAR_RADIX_SORT_HPP
#define LINEAR_RADIX_SORT_HPP
#include "sort/linear_complexity/counting_sort.hpp"

namespace sort
{
template<class T, class U, class Tracer>
void radix_sort(std::vector<T> &A, U max_digit_count, Tracer tracer)
{
    std::vector<T> intermediate_array(A.size(), T{});

    for(U digit = 0; digit < max_digit_count; digit++)
    {
        tracer(digit);
        countingsort(A, intermediate_array, size_t(10), tracer, (10 * digit + 1 ????? 10, 100, 1000));

        A.swap(intermediate_array);
        std::fill(intermediate_array.begin(), intermediate_array.end(), T{});
    }
}
}

#endif //LINEAR_RADIX_SORT_HPP
