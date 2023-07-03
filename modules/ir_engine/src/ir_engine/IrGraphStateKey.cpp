#include "IrGraphStateKey.h"

bool GraphStateKey::operator== (const GraphStateKey & other) const
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

std::size_t GraphStateKeyHashFn::operator() (const GraphStateKey & p) const
{
    if (p.policies.empty ())
        return 0;

    const auto & last_proc = p.policies.back ();
    std::size_t hash = std::hash<int> () (last_proc.processor_index);
    HashCombine (hash, last_proc.cache_policy.GetHashForState (p.graph_state));
    return hash;
}
