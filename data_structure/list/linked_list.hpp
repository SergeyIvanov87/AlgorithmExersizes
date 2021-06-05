#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP
#include <memory>

template<class T>
class linked_list
{
    struct node
    {
        T key;
        std::shared_ptr<node> next_node_ptr;
    };

    using node_ptr = std::shared_ptr<node>;
    node_ptr head_node_ptr;

public:

    node_ptr search(const T& key)
    {
        node_ptr x = head_node_ptr;
        while (x and x->key != key)
        {
            x = x->next;
        }

        return x;
    }

    void insert(node_ptr x) //in beginning
    {
        if (x)
        {
            x->next_node_ptr = head_node_ptr;
        }
        head_node_ptr = x;
    }

    void delete_node(node_ptr x)
    {
        node_ptr prev = head_node_ptr;

        while(prev)
        {
            if(prev->next == x)
            {
                prev->next = x->next;
                return;
            }
            prev = prev->next_node_ptr;
        }
    }

    node_ptr top()
    {
        return head_node_ptr;
    }
};
#endif //LINKED_LIST_HPP
