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
            if (p->left == z)
            {
                p->left = z->right;
            }
            else
            {
                p->right = z->right;
            }

            z->parent.lock().reset();
            z->right.reset();
            return;
        }
        else if(!z->right)
        {
            // no right
            if (p->left == z)
            {
                p->left = z->left;
            }
            else
            {
                p->right = z->left;
            }

            z->parent.lock().reset();
            z->right.reset();
            return;
        }
        else
        {
            //both exist
            node_ptr r = z->right;

            if(!r->left)
            {
                //simple
                node_ptr l = z->left;
                transplant(z, r);
                r->left = l;

                z->parent.lock().reset();
                z->right.reset();
                return;
            }
            else
            {
                //hard

                //find z successor
                node_ptr y = min_impl(r, tracer);
                transplant(y, y->right);
                y->right = r;

                if (p->right == z)
                {
                    p->right = y;
                }
                else
                {
                    p->left = y;

                }
                y->left = z->left;
                y->parent = p;
            }
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
#endif //BINARY_TREE_HPP
