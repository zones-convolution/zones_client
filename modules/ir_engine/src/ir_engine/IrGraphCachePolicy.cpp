#include "IrGraphCachePolicy.h"

std::size_t IrGraphCachePolicy::GetHashForState (const IrGraphState & state) const
{
    return 0;
}

bool IrGraphCachePolicy::StatesMatchWRTPolicy (const IrGraphState & s1,
                                               const IrGraphState & s2) const
{
    return false;
}

bool IrGraphCachePolicy::operator== (const IrGraphCachePolicy & p) const
{
    return policy_identifier_ == p.policy_identifier_;
}

IrGraphCachePolicy
IrGraphCachePolicy::WithPolicyIdentifier (const std::string & policy_identifier) const
{
    auto new_cache_policy = *this;
    new_cache_policy.policy_identifier_ = policy_identifier;
    return new_cache_policy;
}
