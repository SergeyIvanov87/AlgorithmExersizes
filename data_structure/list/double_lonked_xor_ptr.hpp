#ifndef DOUBLE_LINKED_XOR_LIST_HPP
#define DOUBLE_LINKED_XOR_LIST_HPP

#include <bitset>
#include <memory>
#include <string>

template<class T>
class double_linked_xor_list
{
    struct node
    {
        node(T k) : key(k), np()
        {}

        T key;
        using address_t = uint64_t;
        address_t np; //prev^next

        template<class Tracer>
        static address_t addr_from_ptrs(const node* lhs, const node* rhs, Tracer& tracer)
        {
            return reinterpret_cast<address_t>(lhs) ^ reinterpret_cast<address_t>(rhs);
        }

        template<class Tracer>
        static node* ptr_from_addr(address_t addr, const node* second_ptr, Tracer& tracer)
        {
            return reinterpret_cast<node*>(reinterpret_cast<address_t>(second_ptr) ^ addr);
        }

        template<class Tracer>
        static address_t get_new_addr(address_t addr, const node* old_ptr, const node* new_ptr, Tracer& tracer)
        {
            node* initial_ptr = reinterpret_cast<node*>(reinterpret_cast<address_t>(old_ptr) ^ addr);
            return node::addr_from_ptrs(initial_ptr, new_ptr, tracer);
        }

        void dump(std::ostream& out, std::string prefix = std::string("")) const
        {
            char ptr[128] = {0};

            std::bitset<8 * sizeof(size_t)> bits(np);
            /*
            sprintf(ptr, "%p", (prev.lock() ? prev.lock().get() : nullptr));
            out << prefix << "<-PREV : [" << ptr  << "]" << std::endl;


            */
            sprintf(ptr, " [%p", this);
            out << prefix << ptr << " : " + std::to_string(key) << "]" << std::endl;
            out << prefix  << "NP : " << bits << std::endl;

            auto dummy = [] (const auto& v) {};

            sprintf(ptr, "%p", node::ptr_from_addr(np, nullptr, dummy));
            out << prefix << "  HYPOTETHIS : [" << ptr  << "]->"<< std::endl;
        }
    };

/*
 *       <--[ head ]-->
 *                  <--[ 1 ]-->
 *                          <--[ 2 ]-->
 */
    using node_ptr = node*;
    node_ptr head;

public:

    static node* make_node(T key)
    {
        return new node(key);
    }

    double_linked_xor_list()
    {
        head = make_node(0);
    }

    template<class Tracer>
    node_ptr search(const T& key, Tracer& tracer)
    {
        node* x = node::ptr_from_addr(head->np, nullptr, tracer);
        node* prev  = head;
        while (x and x->key != key)
        {
            tracer(x);



            node* next = node::ptr_from_addr(x->np, prev, tracer);
            prev = x;
            x = next;
        }


        return x;
    }

    template<class Tracer>
    void insert(node_ptr x, Tracer& tracer) //in beginning
    {
        tracer(x);


        node* prev = head;
        node* next = nullptr;
        if (head)
        {
            next = node::ptr_from_addr(head->np, nullptr, tracer);

            if(next)
            {
                next->np = node::get_new_addr(next->np, head, x, tracer);
            }
        }
        x->np = node::addr_from_ptrs(prev, next, tracer);
        head->np = node::addr_from_ptrs(nullptr, x, tracer);

        tracer.reset_prefix();
        tracer(x);
    }

    template<class Tracer>
    void delete_node(node_ptr x, Tracer tracer)
    {
        node* left_node = static_cast<node*>(head->np);
        node* prev  = head;
        node* left_next = node::ptr_from_addr(left_node->np, prev, tracer);
        while (left_next and left_next != x)
        {
            prev = left_node;
            left_node = left_next;

            left_next = node::ptr_from_addr(left_node->np, prev, tracer);
        }

        if (!left_node)  // nothing to delete
        {
            return;
        }

        // find right node
        node* right_node = node::ptr_from_addr(x->np, left_node, tracer);

        // we got both left & right nodes
        left_node->np = node::get_new_addr(left_node->np, x, right_node, tracer);
        if (right_node)
        {
            right_node->np = node::get_new_addr(right_node->np, x, left_node, tracer);
        }

        /*if (n == head)
        {
            head = n->next;
        }*/
    }
};

#endif //DOUBLE_LINKED_XOR_LIST_HPP
