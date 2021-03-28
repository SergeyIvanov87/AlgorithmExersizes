#ifndef TIMED_GRAPH_LIST_HPP
#define TIMED_GRAPH_LIST_HPP

#include <cassert>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#include "node.hpp"

namespace timed
{
using node = graph_node<timed_predesc_helper>;
using graph_node_ptr_t = std::shared_ptr<node>;
using graph_node_wptr_t = std::weak_ptr<node>;

class graph_list : public aux_storage<typename node::helper_t>
{
    using adj_list_t =  std::set<graph_node_ptr_t>;
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
                    if(child->get_aux()->is_not_visited())
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
                if (child->get_aux()->is_not_visited())
                {
                    child->get_aux()->set_predescessor(n);
                    stack.push(child);
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
                if (child->get_aux()->is_not_visited())
                {
                    child->get_aux()->visit(n->get_aux()->get_d() + 1);
                    child->get_aux()->set_predescessor(n);
                    queue.push(child);
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
        else if(auto pred = v->get_aux()->get_predescessor(); !pred)
        {
            p(graph_node_ptr_t{});
        }
        else
        {
            print_path(s, pred, p);
            p(v);
        }
    }

private:

};
}
#endif //TIMED_GRAPH_LIST_HPP
