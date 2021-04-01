#include <iostream>

#include <gtest/gtest.h>

#include "graphs/adjacency_graph.hpp"

TEST(graphSuite, bfs_dfs)
{

        auto printer = [](const auto& n)
        {
            if(!n)
            {
                std::cout << "nullptr" << std::endl;
            }
            std::cout << n.get() << ": " << n->value << std::endl;
            return false;
        };

        {
            simple::graph_list graph_main;

            simple::graph_node_ptr_t r = graph_main.create_node("r");
            simple::graph_node_ptr_t s = graph_main.create_node("s");
            simple::graph_node_ptr_t t = graph_main.create_node("t");
            simple::graph_node_ptr_t u = graph_main.create_node("u");
            simple::graph_node_ptr_t v = graph_main.create_node("v");
            simple::graph_node_ptr_t w = graph_main.create_node("w");
            simple::graph_node_ptr_t x = graph_main.create_node("x");
            simple::graph_node_ptr_t y = graph_main.create_node("y");
            simple::graph_node_ptr_t z = graph_main.create_node("z");


            graph_main.fill_graph(r, {v, s});
            graph_main.fill_graph(s, {r, w});
            graph_main.fill_graph(t, {w, x, u});
            graph_main.fill_graph(u, {t, x, y});
            graph_main.fill_graph(v, {r});
            graph_main.fill_graph(w, {s, t, x});
            graph_main.fill_graph(x, {w, t, u, y});
            graph_main.fill_graph(y, {u, x});

            std::cout << "graph BFS: " << std::endl;

            graph_main.bfs(s, printer);
        }


        {
            simple::graph_list graph_main;

            simple::graph_node_ptr_t u = graph_main.create_node("u");
            simple::graph_node_ptr_t v = graph_main.create_node("v");
            simple::graph_node_ptr_t w = graph_main.create_node("w");
            simple::graph_node_ptr_t x = graph_main.create_node("x");
            simple::graph_node_ptr_t y = graph_main.create_node("y");
            simple::graph_node_ptr_t z = graph_main.create_node("z");

            static const bool directional = true;
            graph_main.fill_graph(u, {v, x}, directional);
            graph_main.fill_graph(v, {y}, directional);
            graph_main.fill_graph(w, {y, z}, directional);
            graph_main.fill_graph(x, {v}, directional);
            graph_main.fill_graph(y, {x}, directional);
            graph_main.fill_graph(z, {z}, directional);

            std::cout << "graph DFS: " << std::endl;

            graph_main.dfs(u, printer);

            std::cout << "graph DFS: " << std::endl;
            graph_main.dfs(w, printer);
        }
    }
