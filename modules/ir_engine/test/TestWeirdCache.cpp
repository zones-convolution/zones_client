#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <string>
#include <unordered_map>

struct GraphState
{
    int param_1;
    int param_2;
    int param_3;
};

class IrGraphCachePolicyWeird
{
public:
    [[nodiscard]] std::size_t GetHashForState (const GraphState & state) const
    {
        return std::hash<std::string> {}(policy_identifier);
    }

    bool StatesMatchWRTPolicy (const GraphState & s1, const GraphState & s2) const
    {
        return true;
    }

    bool operator== (const IrGraphCachePolicyWeird & p) const
    {
        return policy_identifier == p.policy_identifier;
    }

    std::string policy_identifier;
};

struct CachedGraphProcessorState
{
    int processor_index;
    GraphState graph_state;
    IrGraphCachePolicyWeird cache_policy;

    bool operator== (const CachedGraphProcessorState & p) const
    {
        auto process_index_match = p.processor_index == processor_index;
        auto cache_policies_match = p.cache_policy == cache_policy;
        auto is_cached_state = cache_policy.StatesMatchWRTPolicy (p.graph_state, graph_state);

        return process_index_match && cache_policies_match && is_cached_state;
    }
};

struct GraphProcessorStateHashFn
{
    std::size_t operator() (const CachedGraphProcessorState & p) const
    {
        std::size_t h1 = std::hash<int> () (p.processor_index);
        std::size_t h2 = p.cache_policy.GetHashForState (p.graph_state);
        return h1 ^ h2;
    }
};

TEST_CASE ("asdasd asdasdasd asdasd")
{
    auto gps_key_1 = CachedGraphProcessorState {.processor_index = 1};
    auto gps_key_2 = CachedGraphProcessorState {.processor_index = 2};
    auto gps_key_3 = CachedGraphProcessorState {
        .processor_index = 1, .cache_policy = IrGraphCachePolicyWeird {.policy_identifier = "3"}};
    auto gps_key_4 = CachedGraphProcessorState {.processor_index = 4};

    std::unordered_map<CachedGraphProcessorState, int, GraphProcessorStateHashFn> u_map = {
        {gps_key_1, 4}, {gps_key_2, 8}, {gps_key_3, 6}, {gps_key_4, 20}};

    REQUIRE (u_map [gps_key_1] == 4);
    REQUIRE (u_map [gps_key_2] == 8);
    REQUIRE (u_map [gps_key_3] == 6);
    REQUIRE (u_map [gps_key_4] == 20);
}