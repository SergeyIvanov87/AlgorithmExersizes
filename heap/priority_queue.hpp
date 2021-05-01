#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <limits>
#include <stdexcept>
#include <string>

#include "heap/heap_operations.hpp"
#include "utils/printer.hpp"

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
        tracer(data, {data.size() -1 }, std::string("Allocate space for new index ") +
                                        ::utils::to_string({data.size() - 1}));

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

        if( cmp(new_value, data[index]))
        {
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                     " incorrect new key. Comparison condition is brokern");
        }

        data[index] = new_value;
        tracer(data, {index}, "Insert item " + std::to_string(new_value));

        index_t i  = index;
        index_t parent_i = utils::parent(i);

        while(i > 0
                and
              cmp(data[utils::parent(i)], data[i]))
        {
            tracer(data, {i, utils::parent(i)}, std::string("Change with parent by indices: ") +
                                                 ::utils::to_string({i, utils::parent(i)}));

            std::swap(data[utils::parent(i)], data[i]);
            tracer(data, {i, i, utils::parent(i)}, "Heap restored: ");

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

        data.pop_back();
        heapify(data, data.size(), 0, tracer, cmp);
        return t;
    }

private:

    array_t<T> data;
    Comparison cmp = Comparison();
};
}
#endif //PRIORITY_QUEUE_HPP
