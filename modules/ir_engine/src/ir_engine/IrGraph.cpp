#include "IrGraph.h"

std::size_t IrGraph::CombineProcessorHashes (size_t previous_processor_hash,
                                             size_t next_processor_hash) const
{
    return previous_processor_hash ^ next_processor_hash;
}

std::size_t IrGraph::GetHashForCachePolicy (const IrGraphCachePolicy & cache_policy,
                                            size_t previous_processor_hash,
                                            const IrGraphState & ir_graph_state) const
{
    auto state_hash = cache_policy.GetHashForState (ir_graph_state);
    return CombineProcessorHashes (previous_processor_hash, state_hash);
}
std::vector<std::size_t> IrGraph::GetHashesForState (const IrGraphState & ir_graph_state) const
{
    std::size_t last_hash = 0;
    std::vector<std::size_t> hashes_for_state;

    for (const auto & [cache_policy, _] : processors_)
    {
        last_hash = GetHashForCachePolicy (cache_policy, last_hash, ir_graph_state);
        hashes_for_state.push_back (last_hash);
    }

    return hashes_for_state;
}
