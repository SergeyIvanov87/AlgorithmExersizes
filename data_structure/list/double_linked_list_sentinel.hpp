#ifndef DOUBLE_LINKED_LIST_HPP
#define DOUBLE_LINKED_LIST_HPP

#include <memory>

template<class T>
class double_linked_list
{
    struct node
    {
        T key;
        std::weak_ptr<node> prev;
        std::shared_ptr<node> next;
    };

    using node_ptr = std::shared_ptr<node>;
    node_ptr sentinel;

public:

    double_linked_list();
    {
        sentinel = std::make_shared<node>(0)
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
    }

    node_ptr search(const T& key)
    {
        node_ptr x = sentinel->next;
        while (x != sentinel and x->key != key)
        {
            x = x->next;
        }

        return x == sentinel ? {} : x;
    }

    void insert(node_ptr n) //in beginning
    {
        n->next = sentinel->next;
        sentinel->next->prev = n;
        sentinel->next = n;
        n->prev = sentinel;
    }

    void delete_node(node_ptr n)
    {
        n->next->prev = n->prev;
        n->prev->next = n->next;
    }
};

#endif //DOUBLE_LINKED_LIST_HPP
