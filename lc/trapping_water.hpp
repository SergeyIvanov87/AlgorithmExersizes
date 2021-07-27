#include <vector>

namespace trap_water
{
int trap_brute_force(std::vector<int>& height)
{
    size_t sum = 0;

    // for each curr_elem_idx get traverse to the left and to the right
    // and find surrounding borders: left max  & right max
    // when get border - just summarize substraction curr height and border
    // to a final result
    for(size_t curr_elem_idx = 0; curr_elem_idx < height.size(); curr_elem_idx++)
    {
        int left_max = 0;
        int right_max = 0;

        for(int j = curr_elem_idx; j >= 0; j--)
        {
            left_max = std::max(left_max, height[j]);
        }

        for(int j = curr_elem_idx; j < height.size(); j++)
        {
            right_max = std::max(right_max, height[j]);
        }
        sum += std::min(left_max, right_max) - height[curr_elem_idx];
    }

    return sum;
}
int trap_linear_add_memory(std::vector<int>& height)
{
    size_t sum = 0;
    std::vector<int> left(height.size(), 0);
    std::vector<int> right(height.size(), 0);

    // remember max height slice in traversing from left to right
    int max_left = 0;
    for(size_t i = 0; i < height.size(); i++)
    {
        max_left = std::max(max_left, height[i]);
        left[i] = max_left;
    }

    // remember max height slice in traversing from right to left
    int max_right = 0;
    for(size_t i = height.size(); i > 0; i--)
    {
        max_right = std::max(max_right, height[i - 1]);
        right[i - 1] = max_right;
    }

    // summarize water drop in every remembered location
    for (size_t i = 0; i < height.size(); i++)
    {
        sum += std::min(left[i], right[i]) - height[i];
    }
    return sum;
}
}
