#ifndef RB_TREE_HPP
#define RB_TREE_HPP


#include <memory>

namespace tree
{
template<class K>
class rb_tree
{
;
public:
    enum class Color
    {
        Reb, Black
    };

    struct node
    {
        node(const K& key_) : key(key_) {}

        K key;

        Color color;

        std::weak_ptr<node> parent;
        std::shared_ptr<node> left;
        std::shared_ptr<node> right;
    };

    using node_ptr = std::shared_ptr<node>;

    static node_ptr make_node(K key_)
    {
        return std::make_shared<node>(key_);
    }

    void insert(node_ptr z)
    {
        node_ptr x = root_node;
        node_ptr prev{};
        while (x)
        {
            prev = x;
            if (z->key < x->key)
            {
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }

        if (!prev)
        {
            root_node = x;
            root_node->color = Color::Black;
            return;
        }

        if (z->key < prev->key)
        {
            prev->left = z;
        }
        else
        {
            prev->right = z;
        }
        z->parent = prev;

        z->color = Color::Reb;

        insert_fixup(z);
    }
private:

    void left_rotare(node_ptr x)
    {
        if (!x)
        {
            return;
        }
        // remember  X->RIGHT node as Y
        node_ptr y = x->right;

        // exchnage nodes
        x->right = y->left;
        if(y->left)
        {
            y->left->parent.lock() = x;
        }


        node_ptr parent = x->parent.lock();
        if(!parent)
        {
            root_node = y;
        }
        else if(parent->left == x)
        {
            parent->left = y;
        }
        else if(parent->right == x)
        {
            parent->right = y;
        }

        y->parent = parent;
        x->parent = y;
    }

    void  right_rotate(node_ptr y)
    {
        node_ptr x = y->left;

        y->left = x->right;
        if (y->left)
        {
            y->left->parent.lock() = y;
        }

        node_ptr parent = y->parent.lock();
        if (!parent)
        {
            root_node = x;
        }
        else if(parent->left == y)
        {
            parent->left = x;
        }
        else if(parent->right == y)
        {
            parent->right = x;
        }
        x->parent = parent;
        y->parent = x;
    }

    void insert_fixup(node_ptr z)
    {
    }
    node_ptr root;
};
}
#endif //RB_TREE_HPP
