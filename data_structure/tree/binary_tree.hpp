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

    template<class Tracer>
    node_ptr min(Tracer tracer)
    {
        return min_impl(root, tracer);
    }

    template<class Tracer>
    node_ptr max(Tracer tracer)
    {
        return max_impl(root, tracer);
    }

    template<class Tracer>
    node_ptr successor(node_ptr x, Tracer tracer)
    {
        if(x->right)
        {
            return min_impl(x->right, tracer);
        }

        auto y = x->parent.lock();
        while (x == y->right)
        {
            x = y;
            y = x->parent.lock();

        }
        return y;
    }

    template<class Tracer>
    node_ptr search(const K& key, Tracer tracer)
    {
        return search_impl(root, key, tracer);
    }

    node_ptr insert(node_ptr x)
    {
        return insert_impl(root, x);
    }

    node_ptr insert_no_recursion(node_ptr z)
    {
        node_ptr y{};
        node_ptr &x = root;
        while (x)
        {
            y = x;
            if (x->key > z->key)
            {
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }

        if (!x)
        {
            z->parent = y;
            x = z;
        }
        return z;
    }


    template<class Tracer>
    void delete_node(node_ptr z, Tracer tracer)
    {
        if (!z)
        {
            return;
        }

        node_ptr p = z->parent.lock();
        if (!z->left)
        {
            // no left
            transplant(z, z->right);
            return;
        }
        else if(!z->right)
        {
            // no right
            transplant(z, z->left);
            return;
        }
        else
        {
            node_ptr y = min_impl(z->right, tracer);
            if(y->parent.lock() != z)
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent.lock() = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent.lock() = y;
        }

        z->parent.lock().reset();
        z->right.reset();
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

    template<class Tracer>
    node_ptr max_impl(node_ptr cur_node, Tracer tracer)
    {
        auto x = cur_node;
        while(x && x->right)
        {
            x = x->right;
        }
        return x;
    }

    template<class Tracer>
    node_ptr min_impl(node_ptr cur_node, Tracer tracer)
    {
        auto x = cur_node;
        while(x && x->left)
        {
            x = x->left;
        }
        return x;
    }

    template<class Tracer>
    node_ptr search_impl(const node_ptr &cur_node, const K &key, Tracer tracer) const
    {
        if (!cur_node)
        {
            tracer.trace(nullptr);
            return {};
        }

        if(cur_node->key == key)
        {
            tracer.trace("find: ", cur_node.get(), ", key: ", key);
            return cur_node;
        }

        if (key < cur_node->key )
        {
            tracer.trace("left: ", cur_node.get(), ", key: ", cur_node->key);
            return search_impl(cur_node->left, key, tracer);
        }

        tracer.trace("right: ", cur_node.get(), ", key: ", cur_node->key);
        return search_impl(cur_node->right, key, tracer);
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


    /*template<class Tracer>
    void delete_node_impl(node_ptr cur_node, node_ptr x, Tracer tracer)
    {

    }*/

    void transplant(node_ptr u, node_ptr v)
    {
        node_ptr p = u->parent.lock();
        if (!p)
        {
            root = v;
        }
        else if (p->left == u)
        {
            p->left = v;
        }
        else
        {
            p->right = v;
        }

        if (v)
        {
            v->parent.lock() = p;
        }
    }
};
}

template<class K>
struct Node: public std::enable_shared_from_this<Node<K>>
{
    std::shared_ptr<Node<K>> l_successor;
    std::shared_ptr<Node<K>> r_successor;
    std::weak_ptr<Node<K>> predescessor;
    K key;

    std::shared_ptr<Node<K>> insert(K val) {
        if (val <= key) {
            if (!l_successor) {
                l_successor = make_node(val);
                l_successor->predescessor = this->shared_from_this();
                return l_successor;
            }
            return l_successor->insert(val);
        }

        if (!r_successor) {
            r_successor = make_node(val);
            r_successor->predescessor = this->shared_from_this();
            return r_successor;
        }
        return r_successor->insert(val);
    }

    static std::shared_ptr<Node<K>> make_node(K k) {
        auto n = std::make_shared<Node<K>>();
        n->key = k;
        return n;
    }

    template<class T>
    void inorder_tree_walk(T tracer)
    {
        if (l_successor) {
            l_successor->inorder_tree_walk(tracer);
        }
        tracer(this->shared_from_this());
        if (r_successor) {
            r_successor->inorder_tree_walk(tracer);
        }
    }

    template<class Tracer>
    void preorder_tree_walk(Tracer tracer)
    {
        tracer(this->shared_from_this());
        if (l_successor) {
            l_successor->preorder_tree_walk(tracer);
        }
        if (r_successor) {
            r_successor->preorder_tree_walk(tracer);
        }
    }

    template<class Tracer>
    void postorder_tree_walk(Tracer tracer)
    {
        if (l_successor) {
            l_successor->postorder_tree_walk(tracer);
        }
        if (r_successor) {
            r_successor->postorder_tree_walk(tracer);
        }
        tracer(this->shared_from_this());
    }

    template<class Tracer>
    std::shared_ptr<Node<K>> search(K k, Tracer t) {
        if (k == this->key) {
            return this->shared_from_this();
        }

        if (k < this->key) {
            if (l_successor) {
                return l_successor->search(k, t);
            }
        }

        if (k > this->key) {
            if (r_successor) {
                return r_successor->search(k, t);
            }
        }
        return {};
    }

    template<class Tracer>
    void delete_node(std::shared_ptr<Node<K>> n, Tracer tracer)
    {
        if (!n) return;

        auto parent = n->predescessor.lock();
        if(parent)
        {
            if( n->key < parent->key) {
                parent->l_successor.reset();
            }
            if( n->key > parent->key) {
                parent->r_successor.reset();
            }
        }
    }
};
#endif //BINARY_TREE_HPP
