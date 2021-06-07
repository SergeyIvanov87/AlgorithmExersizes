#ifndef LINKED_LIST_OPERATIONS_HPP
#define LINKED_LIST_OPERATIONS_HPP

#include "linked_list.hpp"

template<class T>
void reverse(linked_list<T>& list)
{
    linked_list<T> reversed_list;

    auto new_top = reversed_list.top();
    auto x = list.top();

    while(x)
    {
        auto list_next = x->next;

        x->next = new_top;
        new_top = x;

        x = list_next;
    }
}

#endif //LINKED_LIST_OPERATIONS_HPP
