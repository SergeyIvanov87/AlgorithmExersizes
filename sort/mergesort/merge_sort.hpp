#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP


namespace sort
{

template<class T, class Tracer>
void merge(std::vector<T> &A, size_t p, size_t q, size_t r, Tracer& tracer);

template<class T, class Tracer>
void merge_sort(std::vector<T> &A, size_t p, size_t r, Tracer tracer)
{
    tracer.print_array(A, p ,r);
    if (p == r)
    {
        return;
    }

    size_t q = (p + r) / 2;
    tracer.print_partitioned_array(A, p, q, r);

    Tracer left_tracer(tracer);
    left_tracer.add_prefix("Left:");
    merge_sort(A, p, q, left_tracer);

    Tracer right_tracer(tracer);
    right_tracer.add_prefix("Right:");
    merge_sort(A, q+1, r, right_tracer);

    merge(A, p, q, r, tracer);
}


template<class T, class Tracer>
void merge(std::vector<T> &A, size_t p, size_t q, size_t r, Tracer& tracer)
{
    std::vector<T> L(q - p + 1 + 1/*INF*/);
    std::vector<T> R(r - q + 1/*INF*/);

    std::copy(A.begin() + p, A.begin() + q + 1, L.begin());
    L.back() = std::numeric_limits<T>::max();

    std::copy(A.begin() + q + 1, A.begin() + r + 1, R.begin());
    R.back() = std::numeric_limits<T>::max();

    size_t i = 0;
    size_t j = 0;
    for (size_t k = p; k <= r; k++)
    {
        if(L[i] <= R[j])
        {
            A[k] = L[i];
            i++;
        }
        else
        {
            A[k] = R[j];
            j++;
        }
    }
}
}

#endif //MERGE_SORT_HPP
