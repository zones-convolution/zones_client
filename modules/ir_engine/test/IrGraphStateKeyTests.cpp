#include "ir_engine/IrGraphCachePolicy.h"
#include "ir_engine/IrGraphProcessor.h"
#include "ir_engine/IrGraphStateKey.h"

#include <catch2/catch_test_macros.hpp>
#include <string>

using GraphStateCachePolicy = IrGraphCachePolicy<IrGraphState>;

static float CacheParam1 (const IrGraphState & state)
{
    return state.param_1;
}

static int CacheParam2 (const IrGraphState & state)
{
    return state.param_2;
}

static const std::string & CacheParam3 (const IrGraphState & state)
{
    return state.param_3;
}

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
    auto test_policy = GraphStateCachePolicy ().WithPolicyIdentifier ("test_policy");

    auto graph_state_key_1 =
        GraphStateKey ().WithIndexedPolicy ({.cache_policy = test_policy, .processor_index = 1});
    auto hash_1 = GraphStateKeyHashFn () (graph_state_key_1);

    auto graph_state_key_2 =
        GraphStateKey ().WithIndexedPolicy ({.cache_policy = test_policy, .processor_index = 1});
    auto hash_2 = GraphStateKeyHashFn () (graph_state_key_2);

    REQUIRE (hash_1 == hash_2);
    REQUIRE (graph_state_key_1 == graph_state_key_2);
}

TEST_CASE ("graph state keys with different policies dont match")
{
    auto test_policy_1 = GraphStateCachePolicy ().WithPolicyIdentifier ("test_policy_1");
    auto graph_state_key_1 =
        GraphStateKey ().WithIndexedPolicy ({.cache_policy = test_policy_1, .processor_index = 1});

    auto test_policy_2 = GraphStateCachePolicy ().WithPolicyIdentifier ("test_policy_2");
    auto graph_state_key_2 =
        GraphStateKey ().WithIndexedPolicy ({.cache_policy = test_policy_2, .processor_index = 1});

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE ("graph state keys with different indexes but matching policies dont match")
{
    auto test_policy = GraphStateCachePolicy ().WithPolicyIdentifier ("test_policy");
    auto graph_state_key_1 =
        GraphStateKey ().WithIndexedPolicy ({.cache_policy = test_policy, .processor_index = 1});

    auto graph_state_key_2 =
        GraphStateKey ().WithIndexedPolicy ({.cache_policy = test_policy, .processor_index = 2});

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE ("when states are equal but cache policies dont match equality comparison fails")
{
    auto test_policy = GraphStateCachePolicy ().WithPolicyIdentifier ("test_policy");

    auto graph_state_key_1 = GraphStateKey ().WithIndexedPolicy (
        {.cache_policy = test_policy.WithCachedHandle (&CacheParam1), .processor_index = 1});

    auto graph_state_key_2 =
        GraphStateKey ().WithIndexedPolicy ({.cache_policy = test_policy, .processor_index = 1});

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE ("when policies match but states differ wrt to the policy the equality comparison fails")
{
    auto test_policy = GraphStateCachePolicy ()
                           .WithPolicyIdentifier ("test_policy")
                           .WithCachedHandle (&CacheParam1);

    auto graph_state_key_1 =
        GraphStateKey ()
            .WithGraphState ({.param_1 = 1.f})
            .WithIndexedPolicy ({.cache_policy = test_policy, .processor_index = 1});

    auto graph_state_key_2 =
        GraphStateKey ()
            .WithGraphState ({.param_1 = 2.f})
            .WithIndexedPolicy ({.cache_policy = test_policy, .processor_index = 1});

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE (
    "when states are not the same but match with respect to the policy equality comparsion succeeds")
{
    auto test_policy = GraphStateCachePolicy ()
                           .WithPolicyIdentifier ("test_policy")
                           .WithCachedHandle (&CacheParam2);

    auto graph_state_key_1 =
        GraphStateKey ()
            .WithGraphState ({.param_1 = 1.f})
            .WithIndexedPolicy ({.cache_policy = test_policy, .processor_index = 1});

    auto graph_state_key_2 =
        GraphStateKey ()
            .WithGraphState ({.param_1 = 2.f})
            .WithIndexedPolicy ({.cache_policy = test_policy, .processor_index = 1});

    REQUIRE (graph_state_key_1 == graph_state_key_2);
}

TEST_CASE ("when the number of policies are different, keys equality comparison should fail")
{
    auto indexed_policy = IndexedGraphPolicy {
        .cache_policy = GraphStateCachePolicy ().WithPolicyIdentifier ("test_policy"),
        .processor_index = 1};
    auto graph_state_key_1 =
        GraphStateKey ().WithIndexedPolicy (indexed_policy).WithIndexedPolicy (indexed_policy);

    auto graph_state_key_2 = GraphStateKey ()
                                 .WithIndexedPolicy (indexed_policy)
                                 .WithIndexedPolicy (indexed_policy)
                                 .WithIndexedPolicy (indexed_policy);

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE (
    "when first cache policies are state independent and second are dependent policies and states differ WRT the policy then key equality fails")
{
    auto state_independent_indexed_policy = IndexedGraphPolicy {
        .cache_policy = GraphStateCachePolicy ().WithPolicyIdentifier ("test_policy"),
        .processor_index = 1};

    auto state_dependent_indexed_policy =
        IndexedGraphPolicy {.cache_policy = GraphStateCachePolicy ()
                                                .WithPolicyIdentifier ("test_policy")
                                                .WithCachedHandle (&CacheParam3),
                            .processor_index = 1};

    auto graph_state_key_1 = GraphStateKey ()
                                 .WithIndexedPolicy (state_independent_indexed_policy)
                                 .WithIndexedPolicy (state_dependent_indexed_policy);

    auto graph_state_key_2 = GraphStateKey ()
                                 .WithIndexedPolicy (state_independent_indexed_policy)
                                 .WithIndexedPolicy (state_dependent_indexed_policy);

    REQUIRE (graph_state_key_1 == graph_state_key_2);
    auto hash_1 = GraphStateKeyHashFn () (graph_state_key_1);
    auto hash_2 = GraphStateKeyHashFn () (graph_state_key_2);
    REQUIRE (hash_1 == hash_2);

    graph_state_key_1 = graph_state_key_1.WithGraphState ({.param_3 = "equal not are these"});
    graph_state_key_2 = graph_state_key_2.WithGraphState ({.param_3 = "these are not equal"});

    REQUIRE (graph_state_key_1 != graph_state_key_2);
}

TEST_CASE ("test with map... not done")
{
    auto gps_key_1 = GraphStateKey {.policies = {IndexedGraphPolicy {.processor_index = 1}}};
    auto k1 = GraphStateKeyHashFn () (gps_key_1);

    auto gps_key_2 = GraphStateKey {.policies = {IndexedGraphPolicy {.processor_index = 2}}};
    auto k2 = GraphStateKeyHashFn () (gps_key_2);

    auto gps_key_3 = GraphStateKey ().WithIndexedPolicy ({
        .cache_policy = GraphStateCachePolicy ().WithPolicyIdentifier ("identifier"),
        .processor_index = 1,
    });
    auto k3 = GraphStateKeyHashFn () (gps_key_3);

    auto gps_key_4 = GraphStateKey ().WithIndexedPolicy ({.processor_index = 4});
    auto k4 = GraphStateKeyHashFn () (gps_key_4);

    auto gps_key_5 = GraphStateKey ()
                         .WithIndexedPolicy ({.processor_index = 3})
                         .WithIndexedPolicy ({.processor_index = 1});
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
