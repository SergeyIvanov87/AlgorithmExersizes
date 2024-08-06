#include <iostream>

#include <gtest/gtest.h>

#include "utils/tracer.hpp"
#include "utils/printer.hpp"

#include "data_structure/tree/binary_tree.hpp"

namespace tree
{
namespace binary
{
using tree_init_t = std::vector<int>;
using inorder_path_t = std::vector<int>;
using preorder_path_t = std::vector<int>;
using postorder_path_t = std::vector<int>;

using TreeWalkOrderParams = std::tuple<tree_init_t, inorder_path_t, preorder_path_t, postorder_path_t>;

struct TreeWalkOrderFixture : public testing::TestWithParam<TreeWalkOrderParams>{
};


TEST_P(TreeWalkOrderFixture, inorder_walk)
{
    tree_init_t init = std::get<0>(GetParam());
    inorder_path_t in_expected = std::get<1>(GetParam());
    preorder_path_t pre_expected = std::get<2>(GetParam());
    postorder_path_t post_expected = std::get<3>(GetParam());

    binary_tree<int> t;
    for (auto i_ : init)
    {
        t.insert(binary_tree<int>::make_node(i_));
    }

    std::shared_ptr<Node<int>> root;
    for (auto i_ :init)
    {
        if (!root) {
            root = Node<int>::make_node(i_);
        }
        else {
            root->insert(i_);
        }
    }
    std::vector<int> inorder_path;
    t.inorder_tree_walk([&inorder_path](typename binary_tree<int>::node_ptr n)
    {
        if (n)
        {
            inorder_path.push_back(n->key);
        }
    });
    ASSERT_EQ(in_expected, inorder_path);

    {
        std::vector<int> inorder_path;
        root->inorder_tree_walk([&inorder_path](std::shared_ptr<Node<int>> n)
        {
            if (n)
            {
                inorder_path.push_back(n->key);
            }
        });
        ASSERT_EQ(in_expected, inorder_path);
    }


    std::vector<int> preorder_path;
    auto preorder_tracer = [&preorder_path](typename binary_tree<int>::node_ptr n)
    {
        if (n)
        {
            preorder_path.push_back(n->key);
        }
    };
    t.preorder_tree_walk(preorder_tracer);
    ASSERT_EQ(pre_expected, preorder_path);

    {
        std::vector<int> preorder_path;
        auto preorder_tracer = [&preorder_path](std::shared_ptr<Node<int>> n)
        {
            if (n)
            {
                preorder_path.push_back(n->key);
            }
        };
        root->preorder_tree_walk(preorder_tracer);
        ASSERT_EQ(pre_expected, preorder_path);
    }

    std::vector<int> postrder_path;
    auto postorder_tracer = [&postrder_path](typename binary_tree<int>::node_ptr n)
    {
        if (n)
        {
            postrder_path.push_back(n->key);
        }
    };
    t.postorder_tree_walk(postorder_tracer);
    ASSERT_EQ(post_expected, postrder_path);
    {
        std::vector<int> postrder_path;
        auto postorder_tracer = [&postrder_path](std::shared_ptr<Node<int>> n)
        {
            if (n)
            {
                postrder_path.push_back(n->key);
            }
        };
        root->postorder_tree_walk(postorder_tracer);
        ASSERT_EQ(post_expected, postrder_path);
    }
}


INSTANTIATE_TEST_SUITE_P(TreeWalkOrderGroup, TreeWalkOrderFixture,
                         testing::Values(
                            TreeWalkOrderParams{
                                            {1,4,5,10,16,17,21},
                                            {1,4,5,10,16,17,21},
                                            {1,4,5,10,16,17,21},
                                            {21,17,16,10,5,4,1}},
                          TreeWalkOrderParams{
                                            {21,17,16,10,5,4,1},
                                            {1,4,5,10,16,17,21},
                                            {21,17,16,10,5,4,1},
                                            {1,4,5,10,16,17,21}}

                                        ));




using search_key_t = int;
using found_t = bool;

using TreeSearchParams = std::tuple<tree_init_t, search_key_t, found_t>;

struct TreeSearchFixture : public testing::TestWithParam<TreeSearchParams>{
};


TEST_P(TreeSearchFixture, search)
{
    tree_init_t init = std::get<0>(GetParam());
    search_key_t key = std::get<1>(GetParam());
    found_t expected_found = std::get<2>(GetParam());

    binary_tree<int> t;
    for (auto i_ : init)
    {
        t.insert(binary_tree<int>::make_node(i_));
    }

    std::shared_ptr<Node<int>> root;
    for (auto i_ :init)
    {
        if (!root) {
            root = Node<int>::make_node(i_);
        }
        else {
            root->insert(i_);
        }
    }
    StdoutTracer stdout_tracer;
    auto got_node = t.search(key, stdout_tracer);
    ASSERT_TRUE((bool)got_node == expected_found);

    {
        if (root)
        {
            auto got_node = root->search(key, stdout_tracer);
            ASSERT_TRUE((bool)got_node == expected_found);
        }
    }
}

INSTANTIATE_TEST_SUITE_P(TreeSearchGroup, TreeSearchFixture,
                         testing::Values(
                            TreeSearchParams{
                                            {},
                                            0,
                                            false},
                            TreeSearchParams{
                                            {1,4,5,10,16,17,21},
                                            0,
                                            false},
                            TreeSearchParams{
                                            {21,17,16,10,5,4,1},
                                            21,
                                            true},
                            TreeSearchParams{
                                            {21,17,16,10,5,4,1},
                                            1,
                                            true},
                            TreeSearchParams{
                                            {10,17,16,5,4,1,9,6},
                                            9,
                                            true}
                                        ));


//Delete

using tree_init_t = std::vector<int>;
using delete_key_t = int;
using inorder_path_t = std::vector<int>;
using preorder_path_t = std::vector<int>;
using postorder_path_t = std::vector<int>;

using TreeDeleteParams = std::tuple<tree_init_t, delete_key_t, inorder_path_t, preorder_path_t, postorder_path_t>;

struct TreeDeleteFixture : public testing::TestWithParam<TreeDeleteParams>{
};


TEST_P(TreeDeleteFixture, delete)
{
    tree_init_t init = std::get<0>(GetParam());
    delete_key_t delete_key = std::get<1>(GetParam());
    inorder_path_t in_expected = std::get<2>(GetParam());
    preorder_path_t pre_expected = std::get<3>(GetParam());
    postorder_path_t post_expected = std::get<4>(GetParam());

    // fill
    binary_tree<int> t;
    for (auto i_ : init)
    {
        t.insert(binary_tree<int>::make_node(i_));
    }

    std::shared_ptr<Node<int>> root;
    for (auto i_ :init)
    {
        if (!root) {
            root = Node<int>::make_node(i_);
        }
        else {
            root->insert(i_);
        }
    }
    if (!root) {return;}

    // find key to delete
    EmptyTracer empty_tracer;
    auto node_to_delete = t.search(delete_key, empty_tracer);

    // delete node
    StdoutTracer stdout_tracer;
    t.delete_node(node_to_delete, stdout_tracer);

    std::vector<int> inorder_path;
    t.inorder_tree_walk([&inorder_path](typename binary_tree<int>::node_ptr n)
    {
        if (n)
        {
            inorder_path.push_back(n->key);
        }
    });
    ASSERT_EQ(in_expected, inorder_path);

    std::vector<int> preorder_path;
    auto preorder_tracer = [&preorder_path](typename binary_tree<int>::node_ptr n)
    {
        if (n)
        {
            preorder_path.push_back(n->key);
        }
    };
    t.preorder_tree_walk(preorder_tracer);
    ASSERT_EQ(pre_expected, preorder_path);

    std::vector<int> postrder_path;
    auto postorder_tracer = [&postrder_path](typename binary_tree<int>::node_ptr n)
    {
        if (n)
        {
            postrder_path.push_back(n->key);
        }
    };
    t.postorder_tree_walk(postorder_tracer);
    ASSERT_EQ(post_expected, postrder_path);

    {
        // find key to delete
        EmptyTracer empty_tracer;
        auto node_to_delete = root->search(delete_key, empty_tracer);

        // delete node
        StdoutTracer stdout_tracer;
        root->delete_node(node_to_delete, stdout_tracer);

        std::vector<int> inorder_path;
        root->inorder_tree_walk([&inorder_path](std::shared_ptr<Node<int>> n)
        {
            if (n)
            {
                inorder_path.push_back(n->key);
            }
        });
        ASSERT_EQ(in_expected, inorder_path);

        std::vector<int> preorder_path;
        auto preorder_tracer = [&preorder_path](std::shared_ptr<Node<int>> n)
        {
            if (n)
            {
                preorder_path.push_back(n->key);
            }
        };
        root->preorder_tree_walk(preorder_tracer);
        ASSERT_EQ(pre_expected, preorder_path);

        std::vector<int> postrder_path;
        auto postorder_tracer = [&postrder_path](std::shared_ptr<Node<int>> n)
        {
            if (n)
            {
                postrder_path.push_back(n->key);
            }
        };
        root->postorder_tree_walk(postorder_tracer);
        ASSERT_EQ(post_expected, postrder_path);
    }
}

INSTANTIATE_TEST_SUITE_P(TreeDeleteGroup, TreeDeleteFixture,
                         testing::Values(
                            TreeDeleteParams{
                                            {},
                                            0,
                                            {},
                                            {},
                                            {}},
                            TreeDeleteParams{
                                            {1,4,5,10,16,17,21},
                                            10,
                                            {1,4,5,16,17,21},
                                            {1,4,5,16,17,21},
                                            {21,17,16,5,4,1}},
                            TreeDeleteParams{
                                            {21,17,16,10,5,4,1},
                                            10,
                                            {1,4,5,16,17,21},
                                            {21,17,16,5,4,1},
                                            {1,4,5,16,17,21}},
                            TreeDeleteParams{
                                            {210,190/*to_delete*/,200, 195, 205},
                                            190,
                                            {195, 200, 205, 210},
                                            {210, 200, 195, 205},
                                            {195, 205, 200, 210}},
/*  RIGHT child exist
 *                      210                             210
 *                      /                               /
 *                   |*190*|                          200
 *                   /  \                             /  \
 *             nullptr  200                         195   205
 *                      /  \
 *                   195    205
 *
 */
                            TreeDeleteParams{
                                            {210,190/*to_delete*/,180, 175, 185},
                                            190,
                                            {175,180,185,210},
                                            {210,180,175,185},
                                            {175,185,180,210}},
/*  LEFT child exist
 *                      210                             210
 *                      /                               /
 *                   |*190*|                          180
 *                   /  \                             /  \
 *                180   nullptr                     175   185
 *                /  \
 *              175  185
 *
 */
                            TreeDeleteParams{
                                            {210,190/*to_delete*/,180, 200, 205},
                                            190,
                                            {180,200,205,210},
                                            {210,200,180,205},
                                            {180,205,200,210}},
/*  BOTH child exist - SIMPLE
 *                      210                             210
 *                      /                               /
 *                   |*190*|                          200
 *                   /  \                             /  \
 *                180   200                        180   205
 *                      /  \
 *                nullptr   205
 */
                            TreeDeleteParams{
                                            {210,190/*to_delete*/,180, 200,205,195,197},
                                            190,
                                            {180,195,197,200,205,210},
                                            {210,195,180,200,197,205},
                                            {180,197,205,200,195,210}}
/*  BOTH child exist - HARD
 *                      210                             210
 *                      /                               /
 *                   |*190*|                          195
 *                   /   \                            /  \
 *                180    200                        180   200
 *                       /  \                             /  \
 *                     195   205                        197  205
 *                      \
 *                      197
 */
                                        ));

}
}
