#include "ir_engine/IrGraphCachePolicy.h"
#include "ir_engine/IrGraphProcessor.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("ir graph cache policies")
{
    SECTION ("new cache policies are equal")
    {
        IrGraphState state {};

        auto cache_policy_one = IrGraphCachePolicy ();
        auto cache_policy_two = IrGraphCachePolicy ();

        REQUIRE (cache_policy_one.GetHashForState (state) ==
                 cache_policy_two.GetHashForState (state));
        REQUIRE (cache_policy_one == cache_policy_two);
    }

    SECTION ("the same state is matched with respect to a policy")
    {
        IrGraphState state_1 {};
        IrGraphState state_2 {};

        auto cache_policy = IrGraphCachePolicy ();
        REQUIRE (cache_policy.StatesMatchWRTPolicy (state_1, state_2));

        state_2.param_2 = 20;
        REQUIRE (cache_policy.StatesMatchWRTPolicy (state_1, state_2));

        cache_policy = cache_policy.CacheParam2 (true);
        REQUIRE_FALSE (cache_policy.StatesMatchWRTPolicy (state_1, state_2));
    }

    SECTION ("cache policies with different identifiers result in different hashes")
    {
        IrGraphState state {};

        auto cache_policy_one = IrGraphCachePolicy ().WithPolicyIdentifier ("policy_one");
        auto cache_policy_two = IrGraphCachePolicy ().WithPolicyIdentifier ("policy_two");

        REQUIRE_FALSE (cache_policy_one.GetHashForState (state) ==
                       cache_policy_two.GetHashForState (state));
        REQUIRE_FALSE (cache_policy_one == cache_policy_two);
    }

    SECTION ("cache policies with different cached params are not equal")
    {
        IrGraphState state {};
        auto cache_policy_one = IrGraphCachePolicy ().CacheParam2 (true);
        auto cache_policy_two = IrGraphCachePolicy ();

        REQUIRE_FALSE (cache_policy_one.GetHashForState (state) ==
                       cache_policy_two.GetHashForState (state));
        REQUIRE_FALSE (cache_policy_one == cache_policy_two);
    }
}