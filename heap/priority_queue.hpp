#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <limits>
#include <stdexcept>
#include <string>

#include "heap/heap_operations.hpp"

namespace heap
{

template<class T, class Comparison = std::less<T>>
class priority_queue
{
public:

    template<class Tracer>
    void insert(T value, Tracer tracer)
    {
        data.push_back(std::numeric_limits<T>::min());

        change_key(data.size() - 1, value, tracer);
    }

    template<class Tracer>
    void change_key(index_t index, T new_value, Tracer tracer)
    {
        if(index >= data.size())
        {
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + "HEAP size: " +
                                     std::to_string(data.size()) +
                                     " is less than requested: " + std::to_string(index));
        }

        if( !cmp(data[index], new_value))
        {
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                     " incorrect new key. Comparison condition is brokern");
        }

        data[index] = new_value;

        size_t i  = index;
        while(i > 0
                and
              !cmp(data[utils::parent(i)], data[i]))
        {
            std::swap(data[utils::parent(i)], data[i]);
            i = utils::parent(i);
        }
    }

    const T& top() const noexcept
    {
        return data.at(0);
    }

    template<class Tracer>
    T extract_top(Tracer tracer)
    {
        T t = data.at(0);
        data[0] = data[data.size() - 1];

        heapify(data, data.size() - 1, 0, tracer, cmp);
        return t;
    }

private:

    array_t<T> data;
    Comparison cmp = Comparison();
};
}
#endif //PRIORITY_QUEUE_HPP
