#include "IrGraphHashing.h"

std::size_t IrGraphHashing::GetHashForCachePolicy (const IrGraphCachePolicy * cache_policy,
                                                   size_t previous_processor_hash,
                                                   const IrGraphState & ir_graph_state)
{
    HashCombine (previous_processor_hash, cache_policy->GetHashForState (ir_graph_state));
    return previous_processor_hash;
}

std::vector<std::size_t>
IrGraphHashing::GetHashesForState (const IrGraphState & ir_graph_state,
                                   const std::vector<IrGraphCachePolicy *> & cache_policies)
{
    std::size_t last_hash = 0;
    std::vector<std::size_t> hashes_for_state;

    for (const auto * cache_policy : cache_policies)
    {
        last_hash = IrGraphHashing::GetHashForCachePolicy (cache_policy, last_hash, ir_graph_state);
        hashes_for_state.push_back (last_hash);
    }

    return hashes_for_state;
}