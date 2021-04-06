#include <iostream>
#include <sstream>
#include <benchmark/benchmark.h>

#include "graphs/adjacency_short_path_graph.hpp"

auto empty_trans_printer = [](const auto& in, const auto &out) {};

std::stringstream ss;
auto trans_printer = [](const auto& in, const auto &out) {

    if(!in) {
        ss << "\tIN nullptr\n\n" << std::endl;
        return;
    }
    ss << "\n\t" << in.get() << ": " << in->value << ", {" << in->get_aux()->get_sp_estimate() << "}" << ", pred: " << in->get_aux()->get_predescessor().get()
       << "\n---->\n";

    if(!out) {
        ss << "\tOUT nullptr\n\n" << std::endl;
        return;
    }

    ss << "\t" << out.get() << ": " << out->value << ", {" << out->get_aux()->get_sp_estimate() << "}" << ", pred: " << out->get_aux()->get_predescessor().get()
       << "\n\n" << std::endl;
};

static void BellmanFord_ShortPath_Bench(benchmark::State& state) {

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

    for (auto _ : state)
    {
        graph_main.BellmanFord_ShortPath(s, empty_trans_printer);
    }
}
BENCHMARK(BellmanFord_ShortPath_Bench);


static void BellmanFord_ShortPath_Bench_Print_Path(benchmark::State& state) {

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

    for (auto _ : state)
    {
        graph_main.BellmanFord_ShortPath(s, trans_printer);
    }
}
BENCHMARK(BellmanFord_ShortPath_Bench_Print_Path);

static void Dijkstra_ShortPath_Bench(benchmark::State& state) {

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

    for (auto _ : state)
    {
        graph_main.Dijkstra_ShortPath(s, empty_trans_printer);
    }
}
BENCHMARK(Dijkstra_ShortPath_Bench);


BENCHMARK_MAIN();
