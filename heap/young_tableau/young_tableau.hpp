#ifndef YOUNG_TABLEAU
#define YOUNG_TABLEAU

#include <array>
#include <functional>

namespace heap
{
template<class T, int M, int N, class SequencialOrder = std::less<T>>
struct young_tableau
{
    using mat_t = std::vector<T>;

    young_tableau()
    {
        // fill initial table values: -inf or + inf
        data.resize(M * N );
        std::fill(data.begin(), data.end(),
                  std::is_same<SequencialOrder, std::less<T>>::value
                  ?
                  std::numeric_limits<T>::min()
                  :
                  std::numeric_limits<T>::max());

        // keep the slot for a next insertion
        next_free_index = 0;
    }

    const mat_t& get_data() const noexcept
    {
        return data;
    }

    template<class Tracer>
    void insert(T value, Tracer tracer)
    {
        int r = row_index(next_free_index);
        int c = column_index(next_free_index);

        change_key(r, c, value, tracer);

        next_free_index++;
    }



    template<class Tracer>
    void change_key(int row_index, int column_index, T new_value, Tracer tracer)
    {
        if(row_index >= M)
        {
            std::runtime_error(std::string(__PRETTY_FUNCTION__) + "- invalid index: " +
                               std::to_string(row_index) + ", rows count: " + std::to_string(M));
        }

        if(column_index >= N)
        {
            std::runtime_error(std::string(__PRETTY_FUNCTION__) + "- invalid index: " +
                               std::to_string(column_index) + ", columns count: " +
                               std::to_string(N));
        }

        int change_index = row_index * N + column_index;

        // cannot change key for lesser value ( or greater) than before
        if( order(new_value, data[change_index]))
        {
            throw std::runtime_error(std::string(__PRETTY_FUNCTION__) +
                                     " incorrect new key. SequencialOrder condition is broken: old key: " +
                                     std::to_string(data[change_index]) + ", new key: " +
                                     std::to_string(new_value));
        }

        data[change_index] = new_value;

        // inserted key may brake table invariant:
        // next element should be lesser than LEFT
        // and
        // lesser than TOP neighbors
        while(change_index != 0 &&
              (
              order(data[get_top_neighbor_index(change_index)], data[change_index] ) or
              (order(data[get_left_neighbor_index(change_index)], data[change_index]))
              ))
        {
            //swap with LEFT
            std::swap(data[change_index], data[get_left_neighbor_index(change_index)]);

            // go to LEFT and check invariant again
            change_index = get_left_neighbor_index(change_index);
        }
    }

private:

    static int get_top_neighbor_index(int index)
    {
        return index < N ? 0 : index  - N;
    }

    static int get_left_neighbor_index(int index)
    {
        return index == 0 ? 0 : index - 1;
    }

    static int row_index(int index)
    {
        return index / M;
    }

    static int column_index(int index)
    {
        return index % N;
    }

    size_t next_free_index;
    mat_t data;

    SequencialOrder order;
};
}

#endif //YOUNG_TABLEAU
