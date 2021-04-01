#include <iostream>

#include <gtest/gtest.h>

#include "graphs/adjacency_timed_graph.hpp"

TEST(graphTimedSuite, bfs_dfs)
{
        auto printer = [](const auto& n)
        {
            if(!n)
            {
                std::cout << "nullptr" << std::endl;
                return false;
            }
            std::cout << n.get() << ": " << n->value << ", {" << n->get_aux()->get_d() << ", " << n->get_aux()->get_f() << "}" << ", pred: " << n->get_aux()->get_predescessor().get() << std::endl;
            return false;
        };

        {
            timed::graph_list graph_main;

            timed::graph_node_ptr_t r = graph_main.create_node("r");
            timed::graph_node_ptr_t s = graph_main.create_node("s");
            timed::graph_node_ptr_t t = graph_main.create_node("t");
            timed::graph_node_ptr_t u = graph_main.create_node("u");
            timed::graph_node_ptr_t v = graph_main.create_node("v");
            timed::graph_node_ptr_t w = graph_main.create_node("w");
            timed::graph_node_ptr_t x = graph_main.create_node("x");
            timed::graph_node_ptr_t y = graph_main.create_node("y");
            timed::graph_node_ptr_t z = graph_main.create_node("z");


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

            std::cout << "path: " <<std::endl;
            printer(s);
            std::cout << "&\n";
            printer(y);
            std::cout << "is:\n";
            graph_main.print_path(s, y, printer);
        }


        {
            timed::graph_list graph_main;

            timed::graph_node_ptr_t u = graph_main.create_node("u");
            timed::graph_node_ptr_t v = graph_main.create_node("v");
            timed::graph_node_ptr_t w = graph_main.create_node("w");
            timed::graph_node_ptr_t x = graph_main.create_node("x");
            timed::graph_node_ptr_t y = graph_main.create_node("y");
            timed::graph_node_ptr_t z = graph_main.create_node("z");

            static const bool directional = true;
            graph_main.fill_graph(u, {v, x}, directional);
            graph_main.fill_graph(v, {y}, directional);
            graph_main.fill_graph(w, {y, z}, directional);
            graph_main.fill_graph(x, {v}, directional);
            graph_main.fill_graph(y, {x}, directional);
            graph_main.fill_graph(z, {z}, directional);

            std::cout << "graph DFS: " << std::endl;
            graph_main.dfs(u, printer);

            std::cout << "path: " <<std::endl;
            printer(u);
            std::cout << "&\n";
            printer(x);
            std::cout << "is:\n";
            graph_main.print_path(u, x, printer);

            std::cout << "graph DFS recursive: " << std::endl;
            graph_main.dfs_recursive(printer);

            std::cout << "path: " <<std::endl;
            printer(u);
            std::cout << "&\n";
            printer(x);
            std::cout << "is:\n";
            graph_main.print_path(u, x, printer);

            std::cout << "graph DFS: " << std::endl;
            graph_main.dfs(w, printer);
            std::cout << "path: " <<std::endl;
            printer(w);
            std::cout << "&\n";
            printer(z);
            std::cout << "is:\n";
            graph_main.print_path(w, z, printer);
        }
    }
