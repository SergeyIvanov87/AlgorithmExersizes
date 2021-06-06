#include <iostream>

#include <gtest/gtest.h>

#include "utils/tracer.hpp"
#include "utils/printer.hpp"

#include "data_structure/list/double_linked_list.hpp"
#include "data_structure/list/double_linked_list_sentinel.hpp"

template<template<class> class ListImpl>
using list_t = ListImpl<int>;
using initializer_list_t = std::vector<int>;
using search_key_t = int;
using position_t = size_t;

template<template<class> class ListImpl>
using LinkedListSearchParams = std::tuple<list_t<ListImpl>, initializer_list_t, search_key_t, position_t>;


template<template<class> class ListImpl>
struct LinkedListSearchFixture : public testing::TestWithParam<LinkedListSearchParams<ListImpl>>{
    using list_t = ListImpl<int>;
};



namespace trace
{
    struct VisitingTracer
    {
        void reset_visitor_count()
        {
            visit_count = 0;
        }

        size_t get_visit_count() const
        {
            return visit_count;
        }

        template<class T>
        void operator() (const T& node_item)
        {
            visit_count++;
        }

    private:
        size_t visit_count = 0;
    };

    struct OutputTracer : public VisitingTracer
    {
        OutputTracer(std::ostream& out_) : out(out_) {}

        void reset_prefix()
        {
            prefix.clear();
        }

        template<class T>
        void operator() (const T& node_item)
        {
            VisitingTracer::operator ()(node_item);
            if (node_item)
            {
                node_item->dump(out, prefix);
            }
            else
            {
                out << prefix << "nullptr";
            }

            prefix += "\t";
        }
        std::ostream& out;
        std::string prefix;
    };
}

#define TEST_DEFINITION(FIXTURE, method)                                                            \
TEST_P(FIXTURE, search)                                            \
{   \
    auto par = GetParam();  \
    list_t list = std::get<0>(GetParam());    \
    initializer_list_t init = std::get<1>(GetParam());  \
    search_key_t key = std::get<2>(GetParam()); \
    position_t expected_pos = std::get<3>(GetParam());  \
    \
    trace::OutputTracer insertion_tracer(std::cout);    \
    for (auto _i : init)    \
    {   \
        std::cout << "insert: " << _i << std::endl; \
        list.insert(list_t::make_node(_i), insertion_tracer);   \
        insertion_tracer.reset_prefix();    \
        std::cout << std::endl; \
    }   \
    \
    trace::VisitingTracer empty_tracer;     \
    position_t check_position = init.size();    \
    for (auto _i : init)    \
    {   \
        empty_tracer.reset_visitor_count(); \
        list.search(_i, empty_tracer);  \
\
        check_position--;   \
        ASSERT_EQ(check_position,  empty_tracer.get_visit_count()); \
    }   \
\
    trace::OutputTracer tracer(std::cout);  \
    std::cout << std::endl; \
    std::cout << "search key: " << key << std::endl;    \
    auto find_node = list.search(key, tracer);   \
    ASSERT_EQ(tracer.get_visit_count(), expected_pos);  \
    tracer(find_node); \
    std::cout << std::endl; \
}

using DoubleLLSearchParams = LinkedListSearchParams<double_linked_list>;
using DoubleLLSearchFixture = LinkedListSearchFixture<double_linked_list>;
TEST_DEFINITION(DoubleLLSearchFixture, search)

INSTANTIATE_TEST_SUITE_P(DoubleLLGroup, DoubleLLSearchFixture,
                         testing::Values(
                            DoubleLLSearchParams{
                                            {},
                                            {1,2,3,4,5,6,7}, 7, 0},
                            DoubleLLSearchParams{
                                            {},
                                            {1,2,3,4,5,6,7}, 1, 6},
                            DoubleLLSearchParams{
                                            {},
                                            {}, 1, 0},
                            DoubleLLSearchParams{
                                            {},
                                            {5,8,10,4,3,11,7}, 99, 7}
                         ));


using DoubleLLSentinelSearchParams = LinkedListSearchParams<double_linked_sentinel_list>;
using DoubleLLSentinelSearchFixture = LinkedListSearchFixture<double_linked_sentinel_list>;
TEST_DEFINITION(DoubleLLSentinelSearchFixture, search)
INSTANTIATE_TEST_SUITE_P(DoubleLLSentinelGroup, DoubleLLSentinelSearchFixture,
                         testing::Values(
                            DoubleLLSentinelSearchParams{
                                            {},
                                            {1,2,3,4,5,6,7}, 7, 0},
                            DoubleLLSentinelSearchParams{
                                            {},
                                            {1,2,3,4,5,6,7}, 1, 6},
                            DoubleLLSentinelSearchParams{
                                            {},
                                            {}, 1, 0},
                            DoubleLLSentinelSearchParams{
                                            {},
                                            {5,8,10,4,3,11,7}, 99, 7}
                         ));
