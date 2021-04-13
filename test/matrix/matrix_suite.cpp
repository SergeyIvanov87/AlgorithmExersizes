#include <iostream>
#include <numeric>

#include <gtest/gtest.h>

#include "utils/tracer.hpp"
#include "matrix/matrix.hpp"
#include "matrix/maxtrix_mult.hpp"

namespace matrix
{
TEST(MatrixUtilsSuite, build_indices)
{
    size_t N = 10;
    square_matrix_t<size_t> m(N * N);
    std::iota(m.begin(), m.end(), N * N);

    submatrix_t indices = build_indices_submatrix(N);
    ASSERT_EQ(indices.size(), N * N);
    for(size_t i = 0; i < N; i ++)
    {
        for(size_t j = 0; j < N; j++)
        {
            matrix_index index{i, j};
            ASSERT_EQ(indices.at(i * N + j), index);
        }
    }
}

TEST(MatrixUtilsSuite, build_partitions)
{
    size_t N = 10;
    square_matrix_t<size_t> m(N * N);
    std::iota(m.begin(), m.end(), N * N);

    SubMatrixIndices index = SubMatrixIndices::initial_square(N);
    ASSERT_EQ(index.N, N);
    ASSERT_EQ(index.X_Min, 0);
    ASSERT_EQ(index.X_Max, N);
    ASSERT_EQ(index.Y_Min, 0);
    ASSERT_EQ(index.Y_Max, N);
    for(size_t i = 0; i < N; i++)
    {
        for(size_t j = 0; j < N; j++)
        {
            ASSERT_EQ(m.at(index.index(i, j)), N * N + i * N + j);
        }
    }


    SubMatrixIndices i11 = index.split_11();
    ASSERT_EQ(i11.N, index.N);
    ASSERT_EQ(i11.X_Min, 0);
    ASSERT_EQ(i11.X_Max, N / 2);
    ASSERT_EQ(i11.Y_Min, 0);
    ASSERT_EQ(i11.Y_Max, N / 2);
    for(size_t i = 0; i < N / 2; i++)
    {
        for(size_t j = 0; j < N / 2; j++)
        {
            ASSERT_EQ(m.at(i11.index(i, j)), N * N + i * N + j);
        }
    }

    size_t exception_counter = 0;
    for(size_t i = 0; i < N; i++)
    {
        for(size_t j = 0; j < N; j++)
        {
            try
            {
                m.at(i11.index(i, j));
            }
            catch(...)
            {
                exception_counter++;
            }
        }
    }
    ASSERT_EQ(exception_counter, N * N - N * N / 4);


    SubMatrixIndices i12 = index.split_12();
    ASSERT_EQ(i12.N, index.N);
    ASSERT_EQ(i12.X_Min, 0);
    ASSERT_EQ(i12.X_Max, N / 2);
    ASSERT_EQ(i12.Y_Min, N / 2);
    ASSERT_EQ(i12.Y_Max, N);
    for(size_t i = 0; i < N / 2; i++)
    {
        for(size_t j = 0; j < N / 2; j++)
        {
            ASSERT_EQ(m.at(i12.index(i, j)), N * N + i * N + (j + N / 2));
        }
    }

    exception_counter = 0;
    for(size_t i = 0; i < N; i++)
    {
        for(size_t j = 0; j < N; j++)
        {
            try
            {
                m.at(i12.index(i, j));
            }
            catch(...)
            {
                exception_counter++;
            }
        }
    }
    ASSERT_EQ(exception_counter, N * N - N * N / 4);


    SubMatrixIndices i21 = index.split_21();
    ASSERT_EQ(i21.N, index.N);
    ASSERT_EQ(i21.X_Min, N / 2);
    ASSERT_EQ(i21.X_Max, N);
    ASSERT_EQ(i21.Y_Min, 0);
    ASSERT_EQ(i21.Y_Max, N / 2);
    for(size_t i = 0; i < N / 2; i++)
    {
        for(size_t j = 0; j < N / 2; j++)
        {
            ASSERT_EQ(m.at(i21.index(i, j)), N * N + (i + N / 2) * N + j);
        }
    }

    exception_counter = 0;
    for(size_t i = 0; i < N; i++)
    {
        for(size_t j = 0; j < N; j++)
        {
            try
            {
                m.at(i21.index(i, j));
            }
            catch(...)
            {
                exception_counter++;
            }
        }
    }
    ASSERT_EQ(exception_counter, N * N - N * N / 4);

    SubMatrixIndices i22 = index.split_22();
    ASSERT_EQ(i22.N, index.N);
    ASSERT_EQ(i22.X_Min, N / 2);
    ASSERT_EQ(i22.X_Max, N);
    ASSERT_EQ(i22.Y_Min, N / 2);
    ASSERT_EQ(i22.Y_Max, N);
    for(size_t i = 0; i < N / 2; i++)
    {
        for(size_t j = 0; j < N / 2; j++)
        {
            ASSERT_EQ(m.at(i22.index(i, j)), N * N + (i + N / 2) * N + (j + N / 2));
        }
    }

    exception_counter = 0;
    for(size_t i = 0; i < N; i++)
    {
        for(size_t j = 0; j < N; j++)
        {
            try
            {
                m.at(i22.index(i, j));
            }
            catch(...)
            {
                exception_counter++;
            }
        }
    }
    ASSERT_EQ(exception_counter, N * N - N * N / 4);
}


TEST(MatrixUtilsSuite, build_partitions_recursive)
{
    size_t N = 16;
    square_matrix_t<size_t> m(N * N);
    std::iota(m.begin(), m.end(), N * N);

    SubMatrixIndices index = SubMatrixIndices::initial_square(N);
    SubMatrixIndices i12 = index.split_12();
    ASSERT_EQ(i12.N, index.N);
    ASSERT_EQ(i12.X_Min, 0);
    ASSERT_EQ(i12.X_Max, N / 2);
    ASSERT_EQ(i12.Y_Min, N / 2);
    ASSERT_EQ(i12.Y_Max, N);

    SubMatrixIndices i12_11 = i12.split_11();
    ASSERT_EQ(i12_11.N, index.N);
    ASSERT_EQ(i12_11.X_Min, 0);
    ASSERT_EQ(i12_11.X_Max, N / 4);
    ASSERT_EQ(i12_11.Y_Min, N / 2 );
    ASSERT_EQ(i12_11.Y_Max, N / 2 + N / 4);

    SubMatrixIndices i12_12 = i12.split_12();
    ASSERT_EQ(i12_12.N, index.N);
    ASSERT_EQ(i12_12.X_Min, 0);
    ASSERT_EQ(i12_12.X_Max, N / 4);
    ASSERT_EQ(i12_12.Y_Min, N / 2  + N / 4);
    ASSERT_EQ(i12_12.Y_Max, N / 2 + N / 2);

    SubMatrixIndices i12_21 = i12.split_21();
    ASSERT_EQ(i12_21.N, index.N);
    ASSERT_EQ(i12_21.X_Min, N / 4);
    ASSERT_EQ(i12_21.X_Max, N / 2);
    ASSERT_EQ(i12_21.Y_Min, N / 2 );
    ASSERT_EQ(i12_21.Y_Max, N / 2 + N / 4);

    SubMatrixIndices i12_22 = i12.split_22();
    ASSERT_EQ(i12_22.N, index.N);
    ASSERT_EQ(i12_22.X_Min, N / 4);
    ASSERT_EQ(i12_22.X_Max, N / 2);
    ASSERT_EQ(i12_22.Y_Min, N / 2  + N / 4);
    ASSERT_EQ(i12_22.Y_Max, N / 2 + N / 2);
}

TEST(MatrixUtilsSuite, summarize_different_matrices)
{
    size_t N = 10;
    size_t M = 11;
    square_matrix_t<size_t> n(N * N);
    square_matrix_t<size_t> m(M * M);

    bool catched = false;
    try
    {
        auto ret = sum(n, m);
        (void)ret;
    }
    catch(...)
    {
        catched = true;
    }

    ASSERT_TRUE(catched);
}

TEST(MatrixUtilsSuite, summarize_equal_size_matrices)
{
    size_t N = 10;
    square_matrix_t<size_t> n1(N * N, 1);
    square_matrix_t<size_t> n2(N * N, 3);

    bool catched = false;
    try
    {
        auto ret = sum(n1, n2);
        ASSERT_TRUE(std::all_of(ret.begin(), ret.end(), [](auto v) { return v ==4; }));
    }
    catch(...)
    {
        catched = true;
    }

    ASSERT_FALSE(catched);
}


TEST(MatrixMultSuite, mult_square_matrix_recursive_1x1)
{
    size_t N = 1;
    square_matrix_t<size_t> A(N * N, 2);
    square_matrix_t<size_t> B(N * N, 3);

    SubMatrixIndices index = SubMatrixIndices::initial_square(N);
    square_matrix_t<size_t> C = mult_square_matrix_recursive(A, index, B, index);
    ASSERT_EQ(C.size(), 1);
    ASSERT_EQ(C[0], 6);
}

TEST(MatrixMultSuite, mult_square_matrix_recursive_2x2)
{
    size_t N = 2;
    square_matrix_t<size_t> A{1, 2,
                              3, 4};
    square_matrix_t<size_t> B{5, 6,
                              7, 8};

    SubMatrixIndices index = SubMatrixIndices::initial_square(N);
    square_matrix_t<size_t> C = mult_square_matrix_recursive(A, index, B, index);
    ASSERT_EQ(C.size(), N * N);

    ASSERT_EQ(C[0], 19);
    ASSERT_EQ(C[1], 22);
    ASSERT_EQ(C[2], 43);
    ASSERT_EQ(C[3], 50);
}

TEST(MatrixMultSuite, mult_square_matrix_recursive_4x4)
{
    size_t N = 4;
    square_matrix_t<size_t> A{1, 2, 3, 21,
                              4, 5, 6, 24,
                              7, 8, 9, 27,
                              28, 29, 30, 31};
    square_matrix_t<size_t> B{10, 11, 12, 40,
                              13, 14, 15, 43,
                              16, 17, 18, 46,
                              47, 48, 49, 50};

    SubMatrixIndices index = SubMatrixIndices::initial_square(N);
    square_matrix_t<size_t> C = mult_square_matrix_recursive(A, index, B, index);
    ASSERT_EQ(C.size(), N * N);

    ASSERT_EQ(C[0], 1 * 10 + 2 * 13 + 3 * 16 + 21 * 47);
    ASSERT_EQ(C[1], 1 * 11 + 2 * 14 + 3 * 17 + 21 * 48);
    ASSERT_EQ(C[2], 1 * 12 + 2 * 15 + 3 * 18 + 21 * 49);
    ASSERT_EQ(C[3], 1 * 40 + 2 * 43 + 3 * 46 + 21 * 50);

    ASSERT_EQ(C[4], 4 * 10 + 5 * 13 + 6 * 16 + 24 * 47);
    ASSERT_EQ(C[5], 4 * 11 + 5 * 14 + 6 * 17 + 24 * 48);
    ASSERT_EQ(C[6], 4 * 12 + 5 * 15 + 6 * 18 + 24 * 49);
    ASSERT_EQ(C[7], 4 * 40 + 5 * 43 + 6 * 46 + 24 * 50);

    ASSERT_EQ(C[8], 7 * 10 + 8 * 13 + 9 * 16 + 27 * 47);
    ASSERT_EQ(C[9], 7 * 11 + 8 * 14 + 9 * 17 + 27 * 48);
    ASSERT_EQ(C[10], 7 * 12 + 8 * 15 + 9 * 18 + 27 * 49);
    ASSERT_EQ(C[11], 7 * 40 + 8 * 43 + 9 * 46 + 27 * 50);

    ASSERT_EQ(C[12], 28 * 10 + 29 * 13 + 30 * 16 + 31 * 47);
    ASSERT_EQ(C[13], 28 * 11 + 29 * 14 + 30 * 17 + 31 * 48);
    ASSERT_EQ(C[14], 28 * 12 + 29 * 15 + 30 * 18 + 31 * 49);
    ASSERT_EQ(C[15], 28 * 40 + 29 * 43 + 30 * 46 + 31 * 50);
}
}
