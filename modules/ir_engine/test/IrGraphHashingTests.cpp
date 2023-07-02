#include "ir_engine/IrGraphHashing.h"
#include "ir_engine/IrGraphProcessor.h"

#include <catch2/catch_test_macros.hpp>
#include <functional>

class MockCachePolicy : public IrGraphCachePolicy
{
public:
    size_t GetHashForState (const IrGraphState & state) const override
    {
        return std::hash<std::string> {}(mock_policy_name);
    }

    std::string mock_policy_name;
};

TEST_CASE ("gets hashes for sequential cache policies")
{
    SECTION ("returns empty vector when no cache policies are given")
    {
        auto hashes = IrGraphHashing::GetHashesForState ({}, {});
        REQUIRE (hashes.size () == 0);
    }

    SECTION ("sequential identical cache policies are not equal")
    {
        MockCachePolicy cache_policy_1;
        cache_policy_1.mock_policy_name = "cache_policy_1";

        auto cache_policies =
            std::vector<IrGraphCachePolicy *> {&cache_policy_1, &cache_policy_1, &cache_policy_1};
        auto hashes = IrGraphHashing::GetHashesForState ({}, cache_policies);

        REQUIRE (hashes.size () == 3);

        REQUIRE (hashes [0] != hashes [1]);
        REQUIRE (hashes [0] != hashes [2]);
        REQUIRE (hashes [1] != hashes [2]);
    }
}