#include "ir_engine/IrGraphCachePolicy.h"
#include "ir_engine/IrGraphProcessor.h"

#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <string>
#include <unordered_map>

struct CachedProcessorState
{
    IrGraphCachePolicy cache_policy;
    int processor_index;
};

struct CachedGraphProcessorState
{
    IrGraphState graph_state;
    std::vector<CachedProcessorState> cached_graph;

    bool operator== (const CachedGraphProcessorState & p) const
    {
        if (p.cached_graph.size () != cached_graph.size ())
            return false;

        for (auto i = 0; i < cached_graph.size (); ++i)
        {
            auto cached_state = cached_graph [i];
            auto other_cached_state = p.cached_graph [i];

            auto process_index_match =
                other_cached_state.processor_index == cached_state.processor_index;
            auto cache_policies_match =
                other_cached_state.cache_policy == cached_state.cache_policy;

            auto is_cached_state =
                cached_state.cache_policy.StatesMatchWRTPolicy (p.graph_state, graph_state);

            if (! (process_index_match && cache_policies_match && is_cached_state))
                return false;
        }

        return true;
    }
};

struct GraphProcessorStateHashFn
{
    std::size_t operator() (const CachedGraphProcessorState & p) const
    {
        const auto & last_proc = p.cached_graph.back ();
        std::size_t h1 = std::hash<int> () (last_proc.processor_index);
        std::size_t h2 = last_proc.cache_policy.GetHashForState (p.graph_state);
        return h1 ^ h2;
    }
};

TEST_CASE ("asdasd asdasdasd asdasd")
{
    auto gps_key_1 =
        CachedGraphProcessorState {.cached_graph = {CachedProcessorState {.processor_index = 1}}};

    auto k1 = GraphProcessorStateHashFn () (gps_key_1);

    auto gps_key_2 =
        CachedGraphProcessorState {.cached_graph = {CachedProcessorState {.processor_index = 2}}};

    auto k2 = GraphProcessorStateHashFn () (gps_key_2);

    auto gps_key_3 = CachedGraphProcessorState {
        .cached_graph = {CachedProcessorState {
            //.cache_policy = IrGraphCachePolicy {.policy_identifier_ = "3"},
            .processor_index = 1,
        }}};

    auto k3 = GraphProcessorStateHashFn () (gps_key_3);

    auto gps_key_4 =
        CachedGraphProcessorState {.cached_graph = {CachedProcessorState {.processor_index = 4}}};

    auto k4 = GraphProcessorStateHashFn () (gps_key_4);

    std::unordered_map<CachedGraphProcessorState, int, GraphProcessorStateHashFn> u_map = {
        {gps_key_1, 4}, {gps_key_2, 8}, {gps_key_3, 6}, {gps_key_4, 20}};

    REQUIRE (u_map [gps_key_1] == 4);
    REQUIRE (u_map [gps_key_2] == 8);
    REQUIRE (u_map [gps_key_3] == 6);
    REQUIRE (u_map [gps_key_4] == 20);
}

// #include <iostream>
//
// struct MyStruct {
//     int member_1;
//     int member_2;
// };
//
// class MyClass {
// public:
//     bool isMemberDifferent(const MyStruct& myStruct, int MyStruct::* memberPtr, int newValue) {
//         return myStruct.*memberPtr != newValue;
//     }
// };
//
// int main() {
//     MyStruct myStruct = { 10, 20 };
//     MyClass myClass;
//
//     std::cout << "Is member_1 different? " << std::boolalpha <<
//     myClass.isMemberDifferent(myStruct, &MyStruct::member_1, 5) << std::endl; std::cout << "Is
//     member_2 different? " << std::boolalpha << myClass.isMemberDifferent(myStruct,
//     &MyStruct::member_2, 20) << std::endl;
//
//     return 0;
// }