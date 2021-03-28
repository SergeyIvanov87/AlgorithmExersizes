#include <cassert>
#include <list>
#include <map>
#include <queue>
#include <set>
#ifndef SIMPLE_GRAPH_LIST_HPP
#define SIMPLE_GRAPH_LIST_HPP

#include <stack>
#include <vector>

#include "node.hpp"
namespace simple
{
using node = graph_node<helper>;
using graph_node_ptr_t = std::shared_ptr<node>;
using graph_node_wptr_t = std::weak_ptr<node>;

class graph_list : public aux_storage<typename node::helper_t>
{
    using adj_list_t = std::set<graph_node_ptr_t>;
    using adj_table_t = std::map<graph_node_wptr_t, adj_list_t>;
    adj_table_t table;


public:
    template<class ...args_t>
    graph_node_ptr_t create_node(args_t&& ... args)
    {
        auto n = std::make_shared<node>(std::forward<args_t>(args)..., this);
        return n;
    }

    void fill_graph(graph_node_ptr_t s_node, std::initializer_list<graph_node_ptr_t> depends, bool directional = false)
    {
        auto &adj_for_s_node = table[s_node];
        adj_for_s_node.insert(depends.begin(), depends.end());

        if(!directional)
        {
            for(const auto & parent : depends)
            {
                auto &adj_for_parent = table[parent];
                adj_for_parent.insert(s_node);
            }
        }
    }

    template<class Condition>
    graph_node_ptr_t dfs(graph_node_wptr_t s_node, Condition cond)
    {
        clear();

        std::stack<graph_node_ptr_t> stack;
        {
            auto s_node_shared = s_node.lock();
            stack.push(std::move(s_node_shared));
        }

        while (!stack.empty())
        {
            graph_node_ptr_t n = stack.top();
            stack.pop();

            if (!n->get_aux()->is_visited())
            {
                n->get_aux()->visit(true);
            }

            if (cond(n))
            {
                return n;
            }

            const auto& adj_list = table[n];
            for(auto& child : adj_list)
            {
                if (!child->get_aux()->is_visited())
                {
                    stack.push(child);
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
        {
            auto s_node_shared = s_node.lock();
            s_node_shared->get_aux()->visit(true);
            queue.push(std::move(s_node_shared));
        }

        while (!queue.empty())
        {
            graph_node_ptr_t n = queue.front();
            queue.pop();

            if (cond(n))
            {
                return n;
            }
            n->get_aux()->visit(true);

            const auto& adj_list = table[n];
            for(const auto child : adj_list)
            {
                if (!child->get_aux()->is_visited())
                {
                    queue.push(child);
                }
            }
        }
        return {};
    }

};
}

#endif //SIMPLE_GRAPH_LIST_HPP
