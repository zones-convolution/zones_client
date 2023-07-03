#include "ir_engine/IrGraphCachePolicy.h"
#include "ir_engine/IrGraphProcessor.h"

#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <string>

struct IndexedGraphPolicy
{
    IrGraphCachePolicy cache_policy;
    int processor_index;
};

struct GraphStateKey
{
    IrGraphState graph_state;
    std::vector<IndexedGraphPolicy> policies;

    bool operator== (const GraphStateKey & other) const
    {
        if (other.policies.size () != policies.size ())
            return false;

        for (auto i = 0; i < policies.size (); ++i)
        {
            auto indexed_policy = policies [i];
            auto other_indexed_policy = other.policies [i];

            if (other_indexed_policy.processor_index != indexed_policy.processor_index)
                return false;

            if (other_indexed_policy.cache_policy != indexed_policy.cache_policy)
                return false;

            if (! indexed_policy.cache_policy.StatesMatchWRTPolicy (other.graph_state, graph_state))
                return false;
        }

        return true;
    }
};

struct GraphStateKeyHashFn
{
    std::size_t operator() (const GraphStateKey & p) const
    {
        if (p.policies.empty ())
            return 0;

        const auto & last_proc = p.policies.back ();
        std::size_t h1 = std::hash<int> () (last_proc.processor_index);
        std::size_t h2 = last_proc.cache_policy.GetHashForState (p.graph_state);
        return h1 ^ h2; // SHOULD BE COMBINE HASHES...
    }
};

TEST_CASE ("empty keys are equal and have the same hash")
{
    auto graph_state_key_1 = GraphStateKey {};
    auto hash_1 = GraphStateKeyHashFn () (graph_state_key_1);

    auto graph_state_key_2 = GraphStateKey {};
    auto hash_2 = GraphStateKeyHashFn () (graph_state_key_2);

    REQUIRE (hash_1 == hash_2);
    REQUIRE (hash_1 == 0);
    REQUIRE (graph_state_key_1 == graph_state_key_2);
}

TEST_CASE ("two of the same graph state keys are equal and have the same hash")
{
    auto test_policy = IrGraphCachePolicy ().WithPolicyIdentifier ("test_policy");

    auto graph_state_key_1 = GraphStateKey {
        .policies = {IndexedGraphPolicy {.cache_policy = test_policy, .processor_index = 1}}};
    auto hash_1 = GraphStateKeyHashFn () (graph_state_key_1);

    auto graph_state_key_2 = GraphStateKey {
        .policies = {IndexedGraphPolicy {.cache_policy = test_policy, .processor_index = 1}}};
    auto hash_2 = GraphStateKeyHashFn () (graph_state_key_2);

    REQUIRE (hash_1 == hash_2);
    REQUIRE (graph_state_key_1 == graph_state_key_2);
}

TEST_CASE ("graph state keys with different policies dont match")
{
    auto test_policy_1 = IrGraphCachePolicy ().WithPolicyIdentifier ("test_policy_1");
    auto graph_state_key_1 = GraphStateKey {
        .policies = {IndexedGraphPolicy {.cache_policy = test_policy_1, .processor_index = 1}}};

    auto test_policy_2 = IrGraphCachePolicy ().WithPolicyIdentifier ("test_policy_2");
    auto graph_state_key_2 = GraphStateKey {
        .policies = {IndexedGraphPolicy {.cache_policy = test_policy_2, .processor_index = 1}}};

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE ("graph state keys with different indexes but matching policies dont match")
{
    auto test_policy = IrGraphCachePolicy ().WithPolicyIdentifier ("test_policy");
    auto graph_state_key_1 = GraphStateKey {
        .policies = {IndexedGraphPolicy {.cache_policy = test_policy, .processor_index = 1}}};

    auto graph_state_key_2 = GraphStateKey {
        .policies = {IndexedGraphPolicy {.cache_policy = test_policy, .processor_index = 2}}};

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE ("when states are equal but cache policies dont match equality comparison fails")
{
    auto test_policy = IrGraphCachePolicy ().WithPolicyIdentifier ("test_policy");

    auto graph_state_key_1 =
        GraphStateKey {.policies = {IndexedGraphPolicy {
                           .cache_policy = test_policy.CacheParam1 (true), .processor_index = 1}}};

    auto graph_state_key_2 = GraphStateKey {
        .policies = {IndexedGraphPolicy {.cache_policy = test_policy, .processor_index = 1}}};

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE ("when policies match but states differ wrt to the policy the equality comparison fails")
{
    auto test_policy =
        IrGraphCachePolicy ().WithPolicyIdentifier ("test_policy").CacheParam1 (true);

    auto graph_state_key_1 = GraphStateKey {
        .graph_state = {.param_1 = 1.f},
        .policies = {IndexedGraphPolicy {.cache_policy = test_policy, .processor_index = 1}}};

    auto graph_state_key_2 = GraphStateKey {
        .graph_state = {.param_1 = 2.f},
        .policies = {IndexedGraphPolicy {.cache_policy = test_policy, .processor_index = 1}}};

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE (
    "when states are not the same but match with respect to the policy equality comparsion succeeds")
{
    auto test_policy =
        IrGraphCachePolicy ().WithPolicyIdentifier ("test_policy").CacheParam2 (true);

    auto graph_state_key_1 = GraphStateKey {
        .graph_state = {.param_1 = 1.f},
        .policies = {IndexedGraphPolicy {.cache_policy = test_policy, .processor_index = 1}}};

    auto graph_state_key_2 = GraphStateKey {
        .graph_state = {.param_1 = 2.f},
        .policies = {IndexedGraphPolicy {.cache_policy = test_policy, .processor_index = 1}}};

    REQUIRE (graph_state_key_1 == graph_state_key_2);
}

TEST_CASE ("when the number of policies are different, keys equality comparison should fail")
{
    auto indexed_policy = IndexedGraphPolicy {
        .cache_policy = IrGraphCachePolicy ().WithPolicyIdentifier ("test_policy"),
        .processor_index = 1};
    auto graph_state_key_1 = GraphStateKey {.policies = {indexed_policy, indexed_policy}};

    auto graph_state_key_2 =
        GraphStateKey {.policies = {indexed_policy, indexed_policy, indexed_policy}};

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE (
    "when first cache policies are state independent and second are dependent policies and states differ WRT the policy then key equality fails")
{
    auto state_independent_indexed_policy = IndexedGraphPolicy {
        .cache_policy = IrGraphCachePolicy ().WithPolicyIdentifier ("test_policy"),
        .processor_index = 1};

    auto state_dependent_indexed_policy = IndexedGraphPolicy {
        .cache_policy =
            IrGraphCachePolicy ().WithPolicyIdentifier ("test_policy").CacheParam3 (true),
        .processor_index = 1};

    auto graph_state_key_1 = GraphStateKey {
        .policies = {state_independent_indexed_policy, state_dependent_indexed_policy}};

    auto graph_state_key_2 = GraphStateKey {
        .policies = {state_independent_indexed_policy, state_dependent_indexed_policy}};

    REQUIRE (graph_state_key_1 == graph_state_key_2);
    auto hash_1 = GraphStateKeyHashFn () (graph_state_key_1);
    auto hash_2 = GraphStateKeyHashFn () (graph_state_key_2);
    REQUIRE (hash_1 == hash_2);

    graph_state_key_1.graph_state = {.param_3 = "equal not are these"};
    graph_state_key_2.graph_state = {.param_3 = "these are not equal"};

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE ("test with map... not done")
{
    auto gps_key_1 = GraphStateKey {.policies = {IndexedGraphPolicy {.processor_index = 1}}};
    auto k1 = GraphStateKeyHashFn () (gps_key_1);

    auto gps_key_2 = GraphStateKey {.policies = {IndexedGraphPolicy {.processor_index = 2}}};
    auto k2 = GraphStateKeyHashFn () (gps_key_2);

    auto gps_key_3 = GraphStateKey {
        .policies = {IndexedGraphPolicy {
            .cache_policy = IrGraphCachePolicy ().WithPolicyIdentifier ("identifier"),
            .processor_index = 1,
        }}};
    auto k3 = GraphStateKeyHashFn () (gps_key_3);

    auto gps_key_4 = GraphStateKey {.policies = {IndexedGraphPolicy {.processor_index = 4}}};
    auto k4 = GraphStateKeyHashFn () (gps_key_4);

    auto gps_key_5 = GraphStateKey {.policies = {IndexedGraphPolicy {.processor_index = 3},
                                                 IndexedGraphPolicy {.processor_index = 1}}};
    auto k5 = GraphStateKeyHashFn () (gps_key_5);
    REQUIRE (k5 == k1);

    std::unordered_map<GraphStateKey, int, GraphStateKeyHashFn> u_map = {
        {gps_key_1, 4}, {gps_key_2, 8}, {gps_key_3, 6}, {gps_key_4, 20}, {gps_key_5, 550}};

    REQUIRE (u_map [gps_key_1] == 4);
    REQUIRE (u_map [gps_key_2] == 8);
    REQUIRE (u_map [gps_key_3] == 6);
    REQUIRE (u_map [gps_key_4] == 20);
    REQUIRE (u_map [gps_key_5] == 550);
}
