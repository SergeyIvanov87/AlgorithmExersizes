#ifndef HEAP_PROPERTIES_HPP
#define HEAP_PROPERTIES_HPP

#include <cassert>
#include <vector>

namespace heap
{
template<class T>
using array_t = std::vector<T>;
using index_t = size_t;

namespace utils
{
    index_t parent(index_t i)
    {
         return i / 2;
    }

    index_t left(index_t i)
    {
        return i * 2 + 1;
    }

    index_t right(index_t i)
    {
        return i * 2 + 2;
    }
}
}

#endif //HEAP_PROPERTIES_HPP
