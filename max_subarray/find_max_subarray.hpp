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


}
