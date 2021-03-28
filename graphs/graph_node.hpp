#include <cassert>
#include <map>
#include <memory>
#include <queue>
#include <stack>
#include <vector>

#include "ext.hpp"

class graph_node_1 : public std::enable_shared_from_this<graph_node_1> ,
                     helper<graph_node_1>,
                     public payload<int>
{

public:
        using payload<int>::payload;
        using base_t = std::enable_shared_from_this<graph_node_1>;
        using parent_t = std::weak_ptr<graph_node_1>;
        using child_t = std::shared_ptr<graph_node_1>;

        std::vector<child_t> adj_list;

        template<class ...args_t>
        static child_t create(args_t&& ... args)
        {
            return std::make_shared<graph_node_1>(std::forward<args_t>(args)...);
        }


        template<class ...args_t>
        child_t create_child(args_t&& ... args)
        {

            adj_list.push_back(std::make_shared<graph_node_1>(std::forward<args_t>(args)...));

            return adj_list.back();
        }

        child_t get_ptr()
        {
            return shared_from_this();
        }

        template <class Condition>
        child_t dfs(parent_t from, Condition cond)
        {
            std::stack<child_t> stack;
            stack.push(get_ptr());
            while (!stack.empty())
            {
                auto node = stack.top();
                stack.pop();

                if(node->is_visited())
                {
                    continue;
                }

                node->visit(true);
                if (cond(node))
                {
                    return node;
                }

                for(auto n : node->adj_list)
                {
                    stack.push(n);
                }
            }
            return {};
/*
            this->visit(true);
            if( cond(this))
            {
                return get_ptr();
            }

            child_t ret;
            for( const auto& child : adj_list)
            {
                if (child->is_visited())
                {
                    continue;
                }

                ret = child->dfs(get_ptr(), cond);
                if( ret)
                {
                    break;
                }
            }
            return ret;
            */
        }

        template <class Condition>
        child_t bfs(parent_t from, Condition cond)
        {
            std::queue<child_t> queue;
            queue.push(get_ptr());
            while( !queue.empty())
            {
                child_t n = queue.front();
                queue.pop();
                if( n->is_visited() )
                {
                    continue;
                }

                n->visit(true);

                if( cond(n))
                {
                    return n;
                }

                for ( auto cc : n->adj_list)
                {
                    queue.push(cc);
                }
            }
            return {};
        }

        template<class Generator>
        static std::vector<child_t> generate(child_t& s_node, size_t layers, size_t layer_count, Generator gen)
        {
            std::vector<child_t> childs;
            if (layers != 0)
            {
                childs.reserve(layer_count);
                for(size_t i = 0; i < layer_count; i++)
                {
                    auto child = s_node->create_child(gen(layers, layer_count));
                    generate(child, layers - 1, layer_count, gen);
                    childs.push_back(std::move(child));
                }
            }
            return childs;
        }

        friend bool operator< (const parent_t &lhs, const parent_t& rhs)
        {
            return lhs.lock().get() < rhs.lock().get();
        }
private:
    graph_node_1(const graph_node_1&) = default;
    graph_node_1(graph_node_1&&) = default;
    graph_node_1& operator=(const graph_node_1&) = default;
};
