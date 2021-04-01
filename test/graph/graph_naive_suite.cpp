#include <iostream>

#include <gtest/gtest.h>

#include "graphs/graph_node.hpp"

TEST(graphNaiveSuite, bfs_dfs)
{
    auto main_node = graph_node_1::create(0);

    size_t count = 0;
    size_t expected_layers = 4;
    auto gen = [&count] (size_t layers, size_t child_on_layer)
    {
        count = (count+1 ); //% child_on_layer;
        auto ret = (layers) * 1000 + count;
        std::cout << "layer: " << layers << ", value: " << ret << std::endl;
        return ret;
    };

    graph_node_1::generate(main_node, expected_layers, 3, gen);

    // dfs
    auto printer = [](const auto& n)
    {
        std::cout << n.get() << ": " << n->value << std::endl;
        return false;
    };
    std::cout << "B/DFS: " << std::endl;
    main_node->bfs({}, printer);
}
