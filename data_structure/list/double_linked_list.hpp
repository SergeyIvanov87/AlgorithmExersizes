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

        void dump(std::ostream& out, std::string prefix = std::string("")) const
        {
            char ptr[128] = {0};

            sprintf(ptr, "%p", (prev.lock() ? prev.lock().get() : nullptr));
            out << prefix << "<-PREV : [" << ptr  << "]" << std::endl;

            sprintf(ptr, "%p", (next ? next.get() : nullptr));
            out << prefix << "  NEXT : [" << ptr  << "]->"<< std::endl;

            sprintf(ptr, " [%p", this);
            out << prefix << ptr << " : " + std::to_string(key) << "]" << std::endl;
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
    node_ptr search(const T& key, Tracer& tracer)
    {
        node_ptr x = head;
        while (x and x->key != key)
        {
            tracer(x);

            x = x->next;
        }

        return x;
    }

    template<class Tracer>
    void insert(node_ptr n, Tracer& tracer) //in beginning
    {
        tracer(n);

        n->next = head;
        if (head)
        {
            head->prev = n;
        }

        tracer.reset_prefix();
        tracer(n);
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
