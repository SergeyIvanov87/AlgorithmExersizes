#include <iostream>

#include <gtest/gtest.h>

#include "utils/tracer.hpp"
#include "utils/printer.hpp"

#include "data_structure/list/double_linked_list.hpp"

template<template<class> class ListImpl>
using list_t = ListImpl<int>;

using initializer_list_t = std::initializer_list<int>;


using search_key_t = int;
using position_t = size_t;

template<template<class> class ListImpl>
using LinkedListSearchParams = std::tuple<list_t<ListImpl>, initializer_list_t, search_key_t, position_t>;


template<template<class> class ListImpl>
class LinkedListSearchFixture : public testing::TestWithParam<LinkedListSearchParams<ListImpl>> {};



namespace trace
{
}

namespace double_ll
{
using DoubleLLSearchParams = LinkedListSearchParams<double_linked_list>;
using DoubleLLSearchFixture = LinkedListSearchFixture<double_linked_list>;
TEST_P(DoubleLLSearchFixture, search)
{
    list_t<double_linked_list> list = std::get<0>(GetParam());
    initializer_list_t init = std::get<1>(GetParam());
    search_key_t key = std::get<2>(GetParam());
    position_t expected_pos = std::get<3>(GetParam());

    position_t got_pos = 0;
    bool silent = true;

    ::utils::LambdaTracer tracer {
            [&got_pos, &silent](const auto& x)
            {
                got_pos ++;
                if (!silent)
                {
                    std::cout << "node: " << x->to_string() << ", ";
                }
            }};


    // create list
    for (auto _i : init)
    {
        list.insert(list_t<double_linked_list>::make_node(_i));
    }

    // check created list
    position_t check_position = 0;
    for (auto _i : init)
    {
        list.search(_i, tracer);

        check_position++;
        ASSERT_EQ(check_position, got_pos);
    }

    got_pos = 0;
    silent = false;
    list.search(key, tracer);
    ASSERT_EQ(got_pos, expected_pos);
}

INSTANTIATE_TEST_SUITE_P(DoubleLLGroup, DoubleLLSearchFixture,
                         testing::Values(
                            DoubleLLSearchParams{
                                            {},
                                            {1,2,3,4,5,6,7}, 1, 1},
                            DoubleLLSearchParams{
                                            {},
                                            {}, 1, 0},
                            DoubleLLSearchParams{
                                            {},
                                            {5,8,10,4,3,11,7}, 99, 0}
                         ));


}
