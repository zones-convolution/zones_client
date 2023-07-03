#include "IrGraphCachePolicy.h"

std::size_t IrGraphCachePolicy::GetHashForState (const IrGraphState & state) const
{
    auto hash = std::hash<std::string> {}(policy_identifier_);
    if (param_1_cached_)
        HashCombine (hash, state.param_1);
    if (param_2_cached_)
        HashCombine (hash, state.param_2);
    if (param_3_cached_)
        HashCombine (hash, state.param_3);
    return hash;
}

bool IrGraphCachePolicy::StatesMatchWRTPolicy (const IrGraphState & s1,
                                               const IrGraphState & s2) const
{
    if (param_1_cached_ && ! juce::approximatelyEqual (s1.param_1, s2.param_1))
        return false;

    if (param_2_cached_ && s1.param_2 != s2.param_2)
        return false;

    if (param_3_cached_ && s1.param_3 != s2.param_3)
        return false;

    return true;
}

bool IrGraphCachePolicy::operator== (const IrGraphCachePolicy & p) const
{
    return policy_identifier_ == p.policy_identifier_ && param_1_cached_ == p.param_1_cached_ &&
           param_2_cached_ == p.param_2_cached_ && param_3_cached_ == p.param_3_cached_;
}

IrGraphCachePolicy
IrGraphCachePolicy::WithPolicyIdentifier (const std::string & policy_identifier) const
{
    auto new_cache_policy = *this;
    new_cache_policy.policy_identifier_ = policy_identifier;
    return new_cache_policy;
}

[[nodiscard]] IrGraphCachePolicy IrGraphCachePolicy::CacheParam1 (bool is_cached) const
{
    auto new_cache_policy = *this;
    new_cache_policy.param_1_cached_ = is_cached;
    return new_cache_policy;
}

IrGraphCachePolicy IrGraphCachePolicy::CacheParam2 (bool is_cached) const
{
    auto new_cache_policy = *this;
    new_cache_policy.param_2_cached_ = is_cached;
    return new_cache_policy;
}

IrGraphCachePolicy IrGraphCachePolicy::CacheParam3 (bool is_cached) const
{
    auto new_cache_policy = *this;
    new_cache_policy.param_3_cached_ = is_cached;
    return new_cache_policy;
}
