#ifndef LINEAR_COMPLEXITY_SORT_TRACER_IMPL_HPP
#define LINEAR_COMPLEXITY_SORT_TRACER_IMPL_HPP

#include <iostream>

#include "utils/printer.hpp"

namespace sort
{
namespace printer
{
    struct Tracer
    {
        template<class T>
        void cur_element(const std::vector<T>& A, const std::vector<T> &C, size_t cur_index)
        {
            std::cout << "Calculate elements count in ABC set (C)" << std::endl;
            ::utils::highlighter_table_t highlight_indices;
            highlight_indices[cur_index] = {"<", ">"};
            ::utils::print_array_with_indices(std::cout, A, highlight_indices);
            std::cout << "Occurence count of A[" << cur_index << "]=" << A[cur_index] << " in ABC set (C):" << std::endl;
            ::utils::print_array_with_indices(std::cout, C, { static_cast<size_t>(A[cur_index]) });
            std::cout << std::endl;
        }

        template<class T>
        void operator() (const std::vector<T>& A, const std::vector<T> &C, size_t abc_size)
        {
            std::cout << "Non-sorted array (A):" << std::endl;
            ::utils::print_array_with_indices(std::cout, A, { });
            std::cout << "Elements occurences in ABC set (C) FINAL: " << std::endl;
            ::utils::print_array_with_indices(std::cout, C, { });
        }

        template<class T>
        void operator() (const std::vector<T>& A, const std::vector<T> &C)
        {
            std::cout << "Elements count \"not more then next\" distribution in increasing orders (C): " << std::endl;
            ::utils::print_array_with_indices(std::cout, C, { });
        }

        template<class T>
        void place_element(const std::vector<T>& A, const std::vector<T> &B, const std::vector<T> &C, size_t index)
        {
            std::cout << "\n{\n";
            std::cout << "Take " << utils::bold_on << "A[" << index << "]=" << A[index]
                      << utils::bold_off << " from " << utils::bold_on << "non sorted" << utils::bold_off << std::endl;
            ::utils::print_array_with_indices(std::cout, A, { index});

            std::cout << "Put by index C[" << A[index] << "]=" << utils::bold_on << C[A[index]] << utils::bold_off <<" to "
                      << utils::bold_on << "sorted (B)" << utils::bold_off << std::endl;
            ::utils::print_array_with_indices(std::cout, B, { C[A[index] - 1]});

            std::cout << "Reduce occurence count in ABC (C) by 1" << std::endl;
            ::utils::print_array_with_indices(std::cout, C, { A[index]});
            std::cout << "}\n";
        }
    };
}
}
#endif //LINEAR_COMPLEXITY_SORT_TRACER_IMPL_HPP
