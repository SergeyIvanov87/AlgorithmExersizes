#ifndef DIGIT_GAP_HPP
#define DIGIT_GAP_HPP
#include <algorithm>
#include <cstdlib>
#include <cmath>
namespace codility
{
// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;
int digit_gap(std::vector<int> &A) {
    // write your code in C++14 (g++ 6.2.0)

    std::sort(A.begin(), A.end());

    int cand = 1;
    for(size_t i = 0; i < A.size(); i++)
    {
        if(A[i] < cand)
        {
            continue;
        }
        else if(A[i] > cand)
        {
            return cand;
        }
        else
        {
            cand ++;
        }
    }

    return cand;
}
}

#endif //DIGIT_GAP_HPP
