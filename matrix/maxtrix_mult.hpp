#ifndef MULTI_MATRIX_HPP
#define MULTI_MATRIX_HPP

#include "matrix/matrix.hpp"

namespace matrix
{
template<class T>
square_matrix_t<T> mult_square_matrix_recursive(const square_matrix_t<T>& A, const SubMatrixIndices& A_Coord,
                                                const square_matrix_t<T>& B, const SubMatrixIndices& B_Coord)
{
    square_matrix_t<T> CC;
    auto rows  = A_Coord.X_Max - A_Coord.X_Min;

    if(rows == 1)
    {
        CC.push_back(A.at(A_Coord.index(0,0)) * B.at(B_Coord.index(0, 0)));
        return CC;
    }
    else
    {
        SubMatrixIndices A11 = A_Coord.split_11();
        SubMatrixIndices A12 = A_Coord.split_12();
        SubMatrixIndices A21 = A_Coord.split_21();
        SubMatrixIndices A22 = A_Coord.split_22();

        SubMatrixIndices B11 = B_Coord.split_11();
        SubMatrixIndices B12 = B_Coord.split_12();
        SubMatrixIndices B21 = B_Coord.split_21();
        SubMatrixIndices B22 = B_Coord.split_22();

        square_matrix_t<T> C11 = sum(mult_square_matrix_recursive(A, A11, B, B11),
                                       mult_square_matrix_recursive(A, A12, B, B21));

        square_matrix_t<T> C12 = sum(mult_square_matrix_recursive(A, A11, B, B12),
                                     mult_square_matrix_recursive(A, A12, B, B22));

        square_matrix_t<T> C21 = sum(mult_square_matrix_recursive(A, A21, B, B11),
                                     mult_square_matrix_recursive(A, A22, B, B21));

        square_matrix_t<T> C22 = sum(mult_square_matrix_recursive(A, A21, B, B12),
                                     mult_square_matrix_recursive(A, A22, B, B22));

        //C.resize((C_coord.X_Max - C_coord.X_Min) * (C_coord.Y_Max - C_coord.Y_min));
        auto colls  = A_Coord.Y_Max - A_Coord.Y_Min;

        auto sub_colls  = A11.Y_Max - A11.Y_Min;
        CC.resize(colls * colls);


        for (size_t index = 0; index < colls / 2; index ++)
        {

            for (size_t jndex = 0; jndex < colls / 2; jndex ++)
            {
                CC.at(index * colls + jndex) = C11.at(index * sub_colls + jndex);
            }
            for (size_t jndex = colls / 2; jndex < colls; jndex ++)
            {
                CC.at(index * colls + jndex) = C12.at(index * sub_colls + jndex - colls / 2);
            }
        }

        for (size_t index = colls / 2; index < colls; index ++)
        {
            for (size_t jndex = 0; jndex < colls / 2; jndex ++)
            {
                CC.at(index * colls + jndex) = C21.at((index - colls / 2 ) * sub_colls + jndex);
            }
            for (size_t jndex = colls / 2; jndex < colls; jndex ++)
            {
                CC.at(index * colls + jndex) = C22.at((index - colls / 2 )*sub_colls + jndex - colls / 2);
            }
        }
        return CC;
    }
}
/*
template<class T>
void mult_square_matrix(size_t N, const square_matrix_t<T>& A, const square_matrix_t<T>& B, square_matrix_t<T>& C)
{
    submatrix_t indices_submatrix = build_indices_submatrix(N);
    SubMatrixIndices coord = SubMatrixIndices::initial_square(N);
    if(N == 1)
    {
        C[0] = A[0] * B[0];
        return;
    }
    else
    {
        SubMatrixIndices M11 = coord.split_11();
        SubMatrixIndices M12 = coord.split_12();
        SubMatrixIndices M21 = coord.split_21();
        SubMatrixIndices M22 = coord.split_22();

        mult_square_matrix_recursive(C, M11, A, M11, B, M11);
        mult_square_matrix_recursive(C, M11, A, M12, B, M21);

        mult_square_matrix_recursive(C, M12, A, M11, B, M12);
        mult_square_matrix_recursive(C, M12, A, M12, B, M22);

        mult_square_matrix_recursive(C, M21, A, M21, B, M11);
        mult_square_matrix_recursive(C, M21, A, M22, B, M21);

        mult_square_matrix_recursive(C, M22, A, M21, B, M12);
        mult_square_matrix_recursive(C, M22, A, M22, B, M22);
    }
}


void mult_square_matrix_recursive(square_matrix_t<T>& C, const SubMatrixIndices& C_Coord,
                                  const square_matrix_t<T>& A, const SubMatrixIndices& A_Coord,
                                  const square_matrix_t<T>& B, const SubMatrixIndices& B_Coord, bool updateC)
{
    if(C_Coord.X_Max - C_Coord.X_Min == 1)
    {
        //square
        if(!update_C)
        {
            C[C_Coord.index(0,0)] = A[A_Coord.index(0,0)] * B[B_Coord.index(0,0)];
        }
        else
        {
            C[C_Coord.index(0,0)] += A[A_Coord.index(0,0)] * B[B_Coord.index(0,0)];
        }

        return;
    }

    SubMatrixIndices C11 = C_Coord.split_11();
    SubMatrixIndices C12 = C_Coord.split_12();
    SubMatrixIndices C21 = C_Coord.split_21();
    SubMatrixIndices C22 = C_Coord.split_22();

    SubMatrixIndices A11 = A_Coord.split_11();
    SubMatrixIndices A12 = A_Coord.split_12();
    SubMatrixIndices A21 = A_Coord.split_21();
    SubMatrixIndices A22 = A_Coord.split_22();

    SubMatrixIndices B11 = B_Coord.split_11();
    SubMatrixIndices B12 = B_Coord.split_12();
    SubMatrixIndices B21 = B_Coord.split_21();
    SubMatrixIndices B22 = B_Coord.split_22();


    mult_square_matrix_recursive(C, C11, A, A11, B, B11);
    mult_square_matrix_recursive(C, C11, A, A12, B, B21, true);

    mult_square_matrix_recursive(C, C12, A, A11, B, B12);
    mult_square_matrix_recursive(C, C12, A, A12, B, B22, true);

    mult_square_matrix_recursive(C, C21, A, A21, B, B11);
    mult_square_matrix_recursive(C, C21, A, A22, B, B21, true);

    mult_square_matrix_recursive(C, C22, A, A21, B, B12);
    mult_square_matrix_recursive(C, C22, A, A22, B, B22, true);
}*/
}
#endif //MULTI_MATRIX_HPP
