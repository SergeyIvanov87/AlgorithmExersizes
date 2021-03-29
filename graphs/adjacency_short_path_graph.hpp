#ifndef SHORT_PATH_GRAPH_LIST_HPP
#define SHORT_PATH_GRAPH_LIST_HPP

#include <cassert>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#include "node.hpp"

namespace short_path
{
using node = graph_node<shortest_path_helper>;
using graph_node_ptr_t = std::shared_ptr<node>;
using graph_node_wptr_t = std::weak_ptr<node>;

class graph_list : public aux_storage<typename node::helper_t>
{
    using edge_weight_t = typename node::helper_t::estimate_t;
    using adj_list_t =  std::map<graph_node_ptr_t, edge_weight_t>;
    using adj_table_t = std::map<graph_node_wptr_t, adj_list_t>;
    adj_table_t table;


public:
    template<class ...args_t>
    graph_node_ptr_t create_node(args_t&& ... args)
    {
        auto n = std::make_shared<node>(std::forward<args_t>(args)..., this);
        return n;
    }

    void fill_graph(graph_node_ptr_t s_node, std::initializer_list<typename adj_list_t::value_type> depends, bool directional = false)
    {
        auto &adj_for_s_node = table[s_node];
        adj_for_s_node.insert(depends);

        if(!directional)
        {
            for(const typename adj_list_t::value_type &parent : depends)
            {
                auto &adj_for_parent = table[parent.first];
                adj_for_parent.emplace(s_node, parent.second);
            }
        }
    }

    template<class Condition>
    graph_node_ptr_t dfs(graph_node_wptr_t s_node, Condition cond)
    {
        clear();

        std::stack<graph_node_ptr_t> stack;
        size_t time = 0;
        {
            auto s_node_shared = s_node.lock();
            stack.push(std::move(s_node_shared));
        }

        while (!stack.empty())
        {
            time ++;
            graph_node_ptr_t n = stack.top();
            if (n->get_aux()->is_not_visited())
            {
                n->get_aux()->visit(time);
            }
            else if(n->get_aux()->is_visited())
            {
                //do not mark black until all child are not black
                bool all_black = true;
                const auto& adj_list = table[n];
                for(auto & child : adj_list)
                {
                    if(child.first->get_aux()->is_not_visited())
                    {
                        all_black = false;
                        break;
                    }
                }

                if(all_black)
                {
                    n->get_aux()->finish(time);
                    stack.pop();
                    continue;
                }
             }
            else
            {
                stack.pop();
                continue;
            }

            if (cond(n))
            {
                n->get_aux()->finish(time);
                return n;
            }

            const auto& adj_list = table[n];
            for(auto& child : adj_list)
            {
                if (child.first->get_aux()->is_not_visited())
                {
                    child.first->get_aux()->set_predescessor(n);
                    stack.push(child.first);
                    break; //?
                }
            }
        }
        return {};
    }

    template<class Condition>
    graph_node_ptr_t bfs(graph_node_wptr_t s_node, Condition cond)
    {
        clear();


        std::queue<graph_node_ptr_t> queue;

        size_t time = 0;
        {
            auto s_node_shared = s_node.lock();
            s_node_shared->get_aux()->visit(time++);
            queue.push(std::move(s_node_shared));
        }

        while (!queue.empty())
        {
            graph_node_ptr_t n = queue.front();
            queue.pop();

            if (cond(n))
            {
                n->get_aux()->finish(time ++);
                return n;
            }

            const auto& adj_list = table[n];
            for(const auto child : adj_list)
            {
                if (child.first->get_aux()->is_not_visited())
                {
                    child.first->get_aux()->visit(n->get_aux()->get_d() + 1);
                    child.first->get_aux()->set_predescessor(n);
                    queue.push(child.first);
                }
            }

            n->get_aux()->finish(time ++);
        }
        return {};
    }

    template<class Printer>
    void print_path(graph_node_ptr_t s, graph_node_ptr_t v, Printer p)
    {
        if(s == v)
        {
            p(s);
            return;
        }
        else if(auto pred = v->get_aux()->get_predescessor(); pred)
        {
            print_path(s, pred, p);
            p(v);
            return;
        }
        p(graph_node_ptr_t{});
    }


    // shortest path algorithm
    void initialize_single_source(graph_node_ptr_t s)
    {
        clear();
        s->get_aux()->set_sp_estimate(0);
    }

    template<class Tracer>
    void relax(graph_node_ptr_t u, graph_node_ptr_t v, edge_weight_t w, Tracer tracer)
    {
        if(v->get_aux()->get_sp_estimate() > u->get_aux()->get_sp_estimate() + w)
        {
            v->get_aux()->set_sp_estimate(u->get_aux()->get_sp_estimate() + w);
            v->get_aux()->set_predescessor(u);
        }
    }

    template<class Tracer>
    bool BellmanFord_ShortPath(graph_node_ptr_t s, Tracer tracer)
    {
        initialize_single_source(s);

        // for i = 1 to |G.V| - 1
        for ( size_t i = 0; i < table.size() -1 ; i ++)
        {
            for(typename adj_table_t::value_type &adjListValueType : table)
            {
                auto u = adjListValueType.first.lock();

                if(u->get_aux()->get_sp_estimate() == node::helper_t::inf())
                {
                    continue;
                }
                tracer(u, graph_node_ptr_t{});

                // for edge (u,v) -> G,E
                for (auto v_w : adjListValueType.second)
                {
                    auto [v, w] = v_w;
                    tracer(u, v);

                    relax(u, v, w, tracer);

                    tracer(u, v);
                }
            }
        }


        for(typename adj_table_t::value_type &adjListValueType : table)
        {
            auto u = adjListValueType.first.lock();
            for (auto v_w : adjListValueType.second)
            {
                auto [v, w] = v_w;

                if(v->get_aux()->get_sp_estimate() > u->get_aux()->get_sp_estimate() + w)
                {
                    return false;
                }
            }
        }
        return true;
    }

};
}
#endif //SHORT_PATH_GRAPH_LIST_HPP
