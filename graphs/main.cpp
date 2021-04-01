#include <iostream>
#include "graph_node.hpp"
#include "adjacency_graph.hpp"
#include "adjacency_timed_graph.hpp"
#include "adjacency_short_path_graph.hpp"

int main(int argc, char * argv[])
{
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




    {
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
    }
}
