#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <memory>

namespace tree
{
template<class K>
class binary_tree
{
public:
    struct node
    {
        node(const K& key_) : key(key_) {}

        K key;

        std::weak_ptr<node> parent;
        std::shared_ptr<node> left;
        std::shared_ptr<node> right;
    };

    using node_ptr = std::shared_ptr<node>;

    static node_ptr make_node(K key_)
    {
        return std::make_shared<node>(key_);
    }

    node_ptr insert(node_ptr x)
    {
        return insert_impl(root, x);
    }

    template<class T>
    void inorder_tree_walk(T tracer)
    {
        inorder_tree_walk_impl(root, tracer);
    }

    template<class Tracer>
    void preorder_tree_walk(Tracer tracer)
    {
        preorder_tree_walk_impl(root, tracer);
    }

    template<class Tracer>
    void postorder_tree_walk(Tracer tracer)
    {
        postorder_tree_walk_impl(root, tracer);
    }

private:
    node_ptr root;

    template<class Tracer>
    void inorder_tree_walk_impl(node_ptr n, Tracer tracer)
    {
        if (!n)
        {
            return;
        }

        inorder_tree_walk_impl(n->left, tracer);
        tracer(n);
        inorder_tree_walk_impl(n->right, tracer);
    }


    template<class Tracer>
    void preorder_tree_walk_impl(node_ptr n, Tracer tracer)
    {
        if (!n)
        {
            return;
        }
        tracer(n);
        preorder_tree_walk_impl(n->left, tracer);
        preorder_tree_walk_impl(n->right, tracer);
    }

    template<class Tracer>
    void postorder_tree_walk_impl(node_ptr n, Tracer tracer)
    {
        if (!n)
        {
            return;
        }
        postorder_tree_walk_impl(n->left, tracer);
        postorder_tree_walk_impl(n->right, tracer);
        tracer(n);
    }

    node_ptr insert_impl(node_ptr &cur_node, node_ptr x)
    {
        if (!cur_node)
        {
            cur_node = x;
            return cur_node;
        }

        if (cur_node->key > x->key)
        {
            x->parent = cur_node;
            return insert_impl(cur_node->left, x);
        }

        x->parent = cur_node;
        return insert_impl(cur_node->right, x);
    }
};
}
#endif //BINARY_TREE_HPP
