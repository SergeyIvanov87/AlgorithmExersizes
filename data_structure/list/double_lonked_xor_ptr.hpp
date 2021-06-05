#ifndef DOUBLE_LINKED_XOR_LIST_HPP
#define DOUBLE_LINKED_XOR_LIST_HPP

#include <memory>

template<class T>
class double_linked_xor_list
{
    struct node
    {
        T key;
        using address_t = uint64_t;
        address_t np; //prev^next

        static address_t addr_from_ptrs(const node* lhs, const node* rhs)
        {
            return reinterpret_cast<address_t>(lhs) ^ reinterpret_cast<address_of>(rhs);
        }

        static node* ptr_from_addr(address_t addr, const node* second_ptr)
        {
            return reinterpret_cast<node*>(reinterpret_cast<address_t>(second_ptr) ^ addr);
        }

        static address_t get_new_addr(address_t addr, const node* old_ptr, const node* new_ptr)
        {
            node* initial_ptr = reinterpret_cast<node*>(reinterpret_cast<address_t>(old_ptr) ^ addr);
            return node::addr_from_ptrs(initial_ptr, new_ptr);
        }
    };

/*
 *       <--[ head ]-->
 *                  <--[ 1 ]-->
 *                          <--[ 2 ]-->
 */
    using node_ptr = std::shared_ptr<node>;
    node_ptr head;

public:
    double_linked_xor_list()
    {
    }

    node_ptr search(const T& key)
    {
        node* x = static_cast<node*>(head->np);
        node* prev  = head.get();
        while (x and x->key != key)
        {
            node* next = node::ptr_from_addr(x->np, prev);
            prev = x;
            x = next;
        }

        return x;
    }

    void insert(node_ptr x) //in beginning
    {
        node* prev = head.get();
        node* next = nullptr;
        if (head)
        {
            next = head->next;

            next->np = node::get_new_addr(next->np, head.get(), x.get());
        }
        x->np = node::addr_from_ptrs(prev, next);
        head = x;
    }

    void delete_node(node_ptr x)
    {
        node* left_node = static_cast<node*>(head->np);
        node* prev  = head.get();
        node* left_next = node::ptr_from_addr(left_node->np, prev);
        while (left_next and left_next != x.get())
        {
            prev = left_node;
            left_node = left_next;

            left_next = node::ptr_from_addr(left_node->np, prev);
        }

        if (!left_node)  // nothing to delete
        {
            return;
        }

        // find right node
        node* right_node = node::ptr_from_addr(x->np, left_node);

        // we got both left & right nodes
        left_node->np = node::get_new_addr(left_node->np, x.get(), right_node);
        if (right_node)
        {
            right_node->np = node::get_new_addr(right_node->np, x.get(), left_node);
        }

        /*if (n == head)
        {
            head = n->next;
        }*/
    }
};

#endif //DOUBLE_LINKED_XOR_LIST_HPP
