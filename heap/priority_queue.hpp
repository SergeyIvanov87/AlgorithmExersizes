#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <limits>
#include <stdexcept>
#include <string>

#include "heap/heap_operations.hpp"
#include "utils/printer.hpp"

namespace heap
{

template<class T, class SequencialOrder = std::less<T>>
class priority_queue
{
public:

    template<class Tracer>
    void insert(T value, Tracer tracer)
    {
        // prepare slot, push -INF or +INF before
        data.push_back(std::is_same<SequencialOrder, std::less<T>>::value
                        ?
                        std::numeric_limits<T>::min() :
                        std::numeric_limits<T>::max());
        tracer(data, {data.size() -1 }, std::string("Allocate space for new index ") +
                                        ::utils::to_string({data.size() - 1}));

        // upgrade -INF or +INF to actual value
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

        // cannot change key for lesser value ( or greater) than before
        if( order(new_value, data[index]))
        {
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                     " incorrect new key. Comparison condition is brokern");
        }

        data[index] = new_value;
        tracer(data, {index}, "Insert item " + std::to_string(new_value));

        index_t i  = index;
        // inserted key may brake heap invariant:
        // next element should be lesser than PARENT
        while(i > 0
                and
              order(data[utils::parent(i)], data[i]))
        {
            tracer(data, {i, utils::parent(i)}, std::string("Change with parent by indices: ") +
                                                 ::utils::to_string({i, utils::parent(i)}));

            // swap with PARENT, because PARENT is less (or more than new_value)
            std::swap(data[utils::parent(i)], data[i]);
            tracer(data, {i, i, utils::parent(i)}, "Heap restored: ");

            // go to the new PARENT and check invariant again
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
        heapify(data, data.size(), 0, tracer, order);
        return t;
    }

private:

    array_t<T> data;
    SequencialOrder order = SequencialOrder();
};
}
#endif //PRIORITY_QUEUE_HPP
