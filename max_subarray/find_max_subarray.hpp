#include <tuple>
namespace max_subarray {

using index_t = size_t;
using left_index_t = index_t;
using right_index_t = index_t;

template<class T, class Tracer>
std::tuple<left_index_t, right_index_t, T>
find_max_crossing_subarray(const std::vector<T>& array, left_index_t low, size_t mid, right_index_t high, Tracer tracer)
{
    enum
    {
        LOW,
        HIGH,
        SUM
    };

    T left_array_sum = std::numeric_limits<T>::min();
    T sum = 0;
    std::tuple<left_index_t, right_index_t, T> ret_left;
    std::get<HIGH>(ret_left) = mid;
    for(index_t i = mid + 1; i > low; i--)
    {
        sum = sum + array[i - 1];
        if(sum > left_array_sum)
        {
            std::get<LOW>(ret_left) = i - 1;
            std::get<SUM>(ret_left) = sum;
            left_array_sum = sum;
        }
    }

    T right_array_sum = std::numeric_limits<T>::min();
    std::tuple<left_index_t, right_index_t, T> ret_right;
    sum = 0;
    std::get<HIGH>(ret_right) = mid;
    for(index_t i = mid + 1; i <= high; i++)
    {
        sum = sum + array[i];
        if(sum > right_array_sum)
        {
            std::get<HIGH>(ret_right) = i;
            std::get<SUM>(ret_right) = sum;
            right_array_sum = sum;
        }
    }

    tracer.trace("RES: [", low, ";", mid, ";", high, "], sum: ",  std::get<SUM>(ret_left) + std::get<SUM>(ret_right));
    return { std::get<LOW>(ret_left), std::get<HIGH>(ret_right), std::get<SUM>(ret_left) + std::get<SUM>(ret_right) };
}

template<class T, class Tracer>
std::tuple<left_index_t, right_index_t, T>
find_max_subarray(const std::vector<T>& array, left_index_t low, right_index_t high, Tracer tracer)
{
    if(low == high)
    {
        tracer.trace("TERM: [", low, "; ;", high, "], sum: ", array[low]);;
        return { low, high, array[low] };
    }

    index_t mid = (high + low) / 2;

    tracer.trace("LEFT: [", low, ";", mid, "; ]");
    tracer.trace(MakeIncludingRange(array.begin() + low, array.begin() + mid));
    auto [left_array_low, left_array_high, left_array_sum] = find_max_subarray(array, low, mid, tracer);

    tracer.trace("CROSS: [", low, ";", mid, "; ", high, "]");
    tracer.trace(MakeIncludingRange(array.begin() + low, array.begin() + high));
    auto [cross_array_low, cross_array_high, cross_array_sum] = find_max_crossing_subarray(array, low, mid, high, tracer);

    tracer.trace("RIGHT: [", mid + 1, "; ;", high, "]");
    tracer.trace(MakeIncludingRange(array.begin() + mid + 1, array.begin() + high));
    auto [right_array_low, right_array_high, right_array_sum] = find_max_subarray(array, mid + 1, high, tracer);

    left_index_t res_low = left_array_low;
    right_index_t res_right = left_array_high;
    T res_sum = left_array_sum;

    if(left_array_sum < right_array_sum)
    {
        res_sum = right_array_sum;
        res_low = right_array_low;
        res_right = right_array_high;
    }

    if (res_sum < cross_array_sum)
    {
        res_sum = cross_array_sum;
        res_low = cross_array_low;
        res_right = cross_array_high;
    }

    tracer.trace("RES: [", res_low, "; ;", res_right, "], sum: ", res_sum);
    return { res_low, res_right, res_sum };
}

namespace linear
{
template<class T, class Tracer>
std::tuple<left_index_t, right_index_t, T>
find_max_subarray_linear(const std::vector<T>& array, left_index_t low, right_index_t high, Tracer tracer)
{
    if(array.empty())
    {
        throw std::runtime_error("Array is empty");
    }

    left_index_t max_subarray_left = 0, i = std::numeric_limits<T>::min();
    left_index_t max_subarray_right = 0;
    T max_sum = array[max_subarray_left];
    T sum = max_sum;
    T sum_candidate = std::numeric_limits<T>::min();
    for(index_t j = low + 1; j <= high; j++)
    {
        // Max Subarray for A[1...j + 1] is:
        // 1) A[1...j]
        //or
        // 2) A[i...j]
        // where `i` as  1 <= i  <= j + 1

        sum = sum + array[j];
        if(sum > max_sum)
        {
            // it's max then increment `j`
            max_subarray_right = j;
            max_sum = sum;
        }

        //if array[i] is negative then reset index `i`
        if(array[j] < 0)
        {
            i = std::numeric_limits<T>::min();
            sum_candidate = std::numeric_limits<T>::min();
            continue;
        }

        // set `i` positioning as: 1 <= i <= j+1
        // In this case A is first positive value (after latter negative)
        if(i == std::numeric_limits<T>::min())
        {
            i = j;
            sum_candidate = array[j];
        }
        else //another positive A the make addition
        {
            sum_candidate += array[j];
        }

        /*
         * Make comparison subarrays for both
         * A[1...j + 1] and A[i...j + 1]
         */
        if(sum_candidate > sum)
        {
            // exchange `sum` when find better candidate
            max_subarray_left = i;
            max_subarray_right = j;
            sum = sum_candidate;
            max_sum = sum;

            // reset candidate
            i = std::numeric_limits<T>::min();
            sum_candidate = std::numeric_limits<T>::min();
        }
    }

    return {max_subarray_left, max_subarray_right, max_sum};
}
}
}
