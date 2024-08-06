#ifndef OTHERS_TOPK_HPP
#define OTHERS_TOPK_HPP

#include <algorithm>
#include <list>
#include <map>
#include <vector>

namespace others
{
namespace topk
{
template<class T>
using array_t = std::vector<T>;

template<class T>
using list_t = std::list<T>;
using index_t = size_t;

template<class T, class Trace, class Comparator = std::greater<T>>
list_t<index_t> find_topk_indices(const array_t<T>& A, size_t topk_amount, Trace tracer, Comparator comp = Comparator())
{
    using IndecesStorage = std::list<index_t>;
    IndecesStorage indices;

    auto comparator_negation = [comp] (const T &lhs, const T &rhs)
    {
        return not comp(lhs, rhs);
    };
    std::multimap<T, typename IndecesStorage::iterator, decltype(comparator_negation)> topk_element_references(comparator_negation);

    for (size_t i = 0; i < std::min(topk_amount, A.size()); i++)
    {
        indices.push_back(i);
        auto it = topk_element_references.emplace(A[i], std::prev(indices.end()));
        tracer.trace("pivot topk [i: ", *it->second, ", v: ", it->first, "]");
    }
    for (size_t i = topk_amount; i < A.size() && !topk_element_references.empty(); i++)
    {
        const auto &topk_candidate_to_replace_it = topk_element_references.begin();
        tracer.trace("compare topk candidate [i: ", *topk_candidate_to_replace_it->second,
                     ", v: ", topk_candidate_to_replace_it->first, "] to the next element [i: ", i,
                     ", v: ", A[i], "]");
        if (comp(A[i], topk_candidate_to_replace_it->first))
        {
            auto indices_it = topk_candidate_to_replace_it->second;

            tracer.trace("catch! swap: ", *indices_it, " --> ", i);
            *indices_it = i;
            topk_element_references.erase(topk_candidate_to_replace_it);
            topk_element_references.emplace(A[i], indices_it);
        }
    }

    // sort indices in-place
    auto to_inplace_indices_pos = indices.begin();
    for (auto rit = topk_element_references.rbegin(); rit != topk_element_references.rend(); ++rit)
    {
        tracer.trace("for v: ", rit->first, ", i: ", *to_inplace_indices_pos, " <-- ", *rit->second);

        // get max value and put its index in the beginning of indices list
        // if beginning index belongs to max value already - then skip it, and go to a next index
        if (to_inplace_indices_pos == rit->second)
        {
            to_inplace_indices_pos++;
            continue;
        }
        // remember index of max element
        index_t index = *rit->second;

        //swap iterators
        indices.erase(rit->second); //WARNING: std::multimap becomes invalid
        indices.insert(to_inplace_indices_pos, index);
    }
    return indices;
}
}
}
#endif // OTHERS_TOPK_HPP
