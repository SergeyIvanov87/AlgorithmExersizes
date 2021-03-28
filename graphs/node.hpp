#ifndef NODE_HPP
#define NODE_HPP

#include <memory>
#include <string>

#include "ext.hpp"


template<template<class> class THelper>
class graph_node : public std::enable_shared_from_this<graph_node<THelper>> ,
                   public payload<std::string>
{

public:
    using payload<std::string>::payload;
    using value_t = payload<std::string>::value_t;
    using base_t = std::enable_shared_from_this<graph_node>;

    using helper_t = THelper<graph_node>;
    using helper_ptr_t  = std::shared_ptr<helper_t>;

    graph_node(value_t v, aux_storage<helper_t> *_parent) : payload<std::string>(v), parent(_parent) {}


    friend bool operator< (const std::weak_ptr<graph_node> &lhs, const std::weak_ptr<graph_node> &rhs)
    {
        return lhs.lock().get() < rhs.lock().get();
    }

    helper_ptr_t& get_aux()
    {
        if(!aux)
        {
            aux = parent->template create_aux_data();
        }
        return aux;
    }
private:
    graph_node(const graph_node&) = default;
    graph_node(graph_node&&) = default;
    graph_node& operator=(const graph_node&) = default;

    helper_ptr_t aux;
    aux_storage<helper_t> *parent = nullptr;
};

#endif //NODE_HPP
