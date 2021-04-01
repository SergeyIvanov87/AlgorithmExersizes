#include <iostream>

#include <gtest/gtest.h>

#include "graphs/adjacency_short_path_graph.hpp"

        auto printer = [](const auto& n)
        {
            if(!n)
            {
                std::cout << "nullptr" << std::endl;
            }
            std::cout << n.get() << ": " << n->value << ", {" << n->get_aux()->get_d() << ", " << n->get_aux()->get_f() << "}" << ", pred: " << n->get_aux()->get_predescessor().get() << std::endl;
            return false;
        };

        auto trans_printer = [](const auto& in, const auto &out)
        {
            if(!in)
            {
                std::cout << "\tIN nullptr\n\n" << std::endl;
                return;
            }

            std::cout << "\n\t" << in.get() << ": " << in->value << ", {" << in->get_aux()->get_sp_estimate() << "}" << ", pred: " << in->get_aux()->get_predescessor().get()
                      << "\n---->\n";
            if(!out)
            {
                std::cout << "\tOUT nullptr\n\n" << std::endl;
                return;
            }
            std::cout << "\t" << out.get() << ": " << out->value << ", {" << out->get_aux()->get_sp_estimate() << "}" << ", pred: " << out->get_aux()->get_predescessor().get()
                      << "\n\n" << std::endl;
        };
TEST(graphSuite, BellmanFordShortPath)
{



    short_path::graph_list graph_main;

    short_path::graph_node_ptr_t s = graph_main.create_node("s");
    short_path::graph_node_ptr_t t = graph_main.create_node("t");
    short_path::graph_node_ptr_t x = graph_main.create_node("x");
    short_path::graph_node_ptr_t y = graph_main.create_node("y");
    short_path::graph_node_ptr_t z = graph_main.create_node("z");

    static const bool directional = true;
    graph_main.fill_graph(s, { {t, 6}, {y, 7}}, directional);
    graph_main.fill_graph(t, { {x, 5}, {y, 8}, {z, -4}}, directional);
    graph_main.fill_graph(x, { {t, -2}, }, directional);
    graph_main.fill_graph(y, { {x, -3}, {z, 9}}, directional);
    graph_main.fill_graph(z, { {x, 7}, {s, 2}}, directional);

    std::cout << "graph DFS: " << std::endl;
    graph_main.dfs(s, printer);

    std::cout << "Bellman-Ford" << std::endl;
    graph_main.BellmanFord_ShortPath(s, trans_printer);

    std::cout << "print s - z" << std::endl;
    graph_main.print_path(s, z, printer);
}

TEST(graphSuite, DijkstraShortPath)
{
    short_path::graph_list graph_main;

    short_path::graph_node_ptr_t s = graph_main.create_node("s");
    short_path::graph_node_ptr_t t = graph_main.create_node("t");
    short_path::graph_node_ptr_t x = graph_main.create_node("x");
    short_path::graph_node_ptr_t y = graph_main.create_node("y");
    short_path::graph_node_ptr_t z = graph_main.create_node("z");

    static const bool directional = true;
    graph_main.fill_graph(s, { {t, 10}, {y, 5}}, directional);
    graph_main.fill_graph(t, { {x, 1}, {y, 2}}, directional);
    graph_main.fill_graph(x, { {z, 4}, }, directional);
    graph_main.fill_graph(y, { {t, 3}, {x, 9}, {z, 2}}, directional);
    graph_main.fill_graph(z, { {x, 6}, {s, 7}}, directional);

    std::cout << "graph DFS: " << std::endl;
    graph_main.dfs(s, printer);

    std::cout << "Dijkstra" << std::endl;
    graph_main.Dijkstra_ShortPath(s, trans_printer);

    std::cout << "print s - z" << std::endl;
    graph_main.print_path(s, z, printer);
}
