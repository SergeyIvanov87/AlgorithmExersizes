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


}
}
