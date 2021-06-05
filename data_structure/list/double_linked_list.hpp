#ifndef DOUBLE_LINKED_LIST_HPP
#define DOUBLE_LINKED_LIST_HPP

#include <memory>
#include <string>


template<class T>
class double_linked_list
{
    struct node
    {
        node(T k) : key(k)
        {}

        T key;
        std::weak_ptr<node> prev;
        std::shared_ptr<node> next;

        std::string to_string() const
        {
            char ptr[sizeof(size_t)] = {0};
            sprintf(ptr, "%p", this);

            std::string ret(ptr);
            ret += " : " + std::to_string(key);
            return ret;
        }
    };

    using node_ptr = std::shared_ptr<node>;
    node_ptr head;

public:

    static std::shared_ptr<node> make_node(T key)
    {
        return std::make_shared<node>(key);
    }

    template<class Tracer>
    node_ptr search(const T& key, Tracer tracer)
    {
        node_ptr x = head;
        while (x and x->key != key)
        {
            tracer(x);

            x = x->next;
        }

        return x;
    }

    void insert(node_ptr n) //in beginning
    {
        n->next = head;
        if (head)
        {
            head->prev = n;
        }
        head = n;
    }

    void delete_node(node_ptr n)
    {
        if (n->next)
        {
            n->next->prev = n->prev;
        }
        if (n->prev)
        {
            n->prev->next = n->next;
        }

        if (n == head)
        {
            head = n->next;
        }
    }
};

#endif //DOUBLE_LINKED_LIST_HPP
