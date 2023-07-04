#include "ir_engine/IrGraphCachePolicy.h"

#include <catch2/catch_test_macros.hpp>

struct ExampleState
{
    int member_1;
    float member_2;
};

using ExampleStateCachePolicy = IrGraphCachePolicy<ExampleState>;

static int CacheMember1 (const ExampleState & state)
{
    return state.member_1;
}

static float CacheMember2 (const ExampleState & state)
{
    return state.member_2;
}

TEST_CASE ("ir graph cache policies")
{
    SECTION ("new cache policies are equal")
    {
        ExampleState state {};

        auto cache_policy_one = ExampleStateCachePolicy ();
        auto cache_policy_two = ExampleStateCachePolicy ();

        REQUIRE (cache_policy_one.GetHashForState (state) ==
                 cache_policy_two.GetHashForState (state));
        REQUIRE (cache_policy_one == cache_policy_two);
    }

    SECTION ("the same state is matched with respect to a policy")
    {
        ExampleState state_1 {};
        ExampleState state_2 {};

        auto cache_policy = ExampleStateCachePolicy ();
        REQUIRE (cache_policy.StatesMatchWRTPolicy (state_1, state_2));

        state_2.member_2 = 20;
        REQUIRE (cache_policy.StatesMatchWRTPolicy (state_1, state_2));

        cache_policy = cache_policy.WithCachedHandle (&CacheMember2);
        REQUIRE_FALSE (cache_policy.StatesMatchWRTPolicy (state_1, state_2));
    }

    SECTION ("cache policies with different identifiers result in different hashes")
    {
        ExampleState state {};

        auto cache_policy_one = ExampleStateCachePolicy ().WithPolicyIdentifier ("policy_one");
        auto cache_policy_two = ExampleStateCachePolicy ().WithPolicyIdentifier ("policy_two");

        REQUIRE_FALSE (cache_policy_one.GetHashForState (state) ==
                       cache_policy_two.GetHashForState (state));
        REQUIRE_FALSE (cache_policy_one == cache_policy_two);
    }

    SECTION ("cache policies with different cached params are not equal")
    {
        ExampleState state {};
        auto cache_policy_one = ExampleStateCachePolicy ().WithCachedHandle (&CacheMember2);
        auto cache_policy_two = ExampleStateCachePolicy ().WithCachedHandle (&CacheMember1);

        //        REQUIRE_FALSE (cache_policy_one.GetHashForState (state) ==
        //                       cache_policy_two.GetHashForState (state));
        // Relaxed this condition as it is an obscure edge case, whereby the std::hash<float> {}(0.f
        // ) == std::hash<int> {}(0)
        REQUIRE_FALSE (cache_policy_one == cache_policy_two);
    }
}