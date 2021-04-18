#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cassert>
#include <cmath>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <utility>

namespace matrix
{
template<class T>
using square_matrix_t = std::vector<T>;

using index_t = size_t;

struct matrix_index
{
    index_t x;
    index_t y;

    friend bool operator==(const matrix_index& lhs, const matrix_index& rhs)
    {
        return (lhs.x == rhs.x ) && ( lhs.y == rhs.y);
    }
};

using submatrix_t = std::vector<matrix_index>;
submatrix_t build_indices_submatrix(index_t N)
{
    submatrix_t indices_matrix(N * N);
    for(index_t row = 0; row < N; row++)
    {
        for(index_t coll = row; coll < N; coll++)
        {
            indices_matrix[row * N + coll] = matrix_index{row, coll};
            indices_matrix[coll * N + row] = matrix_index{coll, row};
        }
    }
    return indices_matrix;
}

/*
  ----------->y
 |
 |
 |
 |
\/
x
*/
struct SubMatrixIndices
{

    index_t N;
    index_t X_Min, X_Max;
    index_t Y_Min, Y_Max;

    size_t index(index_t x, index_t y) const
    {
        if(x + X_Min >= X_Max)
        {
            throw std::runtime_error(std::string("X is out of range: ") + std::to_string(x) +
                                     ", in index: " + to_string());
        }
        if(y + Y_Min >= Y_Max)
        {
            throw std::runtime_error(std::string("Y is out of range: ") + std::to_string(y) +
                                     ", in index: " + to_string());
        }
        return (x + X_Min) * N + Y_Min + y;
    }

    static SubMatrixIndices initial_square(index_t N)
    {
        SubMatrixIndices m00 {N};
        m00.X_Min = 0;
        m00.Y_Min = 0;
        return m00;
    }

    SubMatrixIndices split_11() const
    {
        SubMatrixIndices m11 {N};
        m11.X_Min = this->X_Min;
        m11.X_Max = (this->X_Min + this->X_Max) / 2;

        m11.Y_Min = this->Y_Min;
        m11.Y_Max = (this->Y_Min + this->Y_Max) / 2;
        return m11;
    }

    SubMatrixIndices split_12() const
    {
        SubMatrixIndices m12 {N};
        m12.X_Min = this->X_Min;
        m12.X_Max = (this->X_Min + this->X_Max) / 2;

        m12.Y_Min = (this->Y_Min + this->Y_Max) / 2;
        m12.Y_Max = this->Y_Max;
        return m12;
    }

    SubMatrixIndices split_21() const
    {
        SubMatrixIndices m21 {N};
        m21.X_Min = (this->X_Min + this->X_Max) / 2;
        m21.X_Max = this->X_Max;

        m21.Y_Min = this->Y_Min;
        m21.Y_Max = (this->Y_Min + this->Y_Max) / 2;
        return m21;
    }

    SubMatrixIndices split_22() const
    {
        SubMatrixIndices m22 {N};
        m22.X_Min = (this->X_Min + this->X_Max) / 2;
        m22.X_Max = this->X_Max;

        m22.Y_Min = (this->Y_Min + this->Y_Max) / 2;
        m22.Y_Max = this->Y_Max;
        return m22;
    }

    std::string to_string() const
    {
        std::stringstream ss;
        ss << "N: " << N << ", X{" << X_Min << ", " << X_Max << "}, "
           << "Y{" << Y_Min << ", " << Y_Max << "}";
        return ss.str();
    }
private:

    explicit SubMatrixIndices(index_t N_) : N(N_), X_Min(N), X_Max(N), Y_Min(N), Y_Max(N) {}
};

namespace details
{
    template <class T, class Op>
    square_matrix_t<T> op_impl(const square_matrix_t<T>& A, const square_matrix_t<T>& B, Op operation)
    {
        if(A.size() != B.size())
        {
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                     " - Unexpected sizes! A size: " + std::to_string(A.size()) +
                                     ", B size:" + std::to_string(B.size()));
        }
        square_matrix_t<T> C;
        C.reserve(A.size());

        std::transform(A.begin(), A.end(), B.begin(), std::back_inserter(C), [operation] (T lhs, T rhs)
        {
            return operation(lhs, rhs);
        });

        return C;
    }
}

template<class T>
square_matrix_t<T> sum(const square_matrix_t<T>& A, const square_matrix_t<T>& B)
{
    return details::op_impl(A, B, std::plus<T>{});
}
}
#endif //MATRIX_HPP
