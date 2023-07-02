#include "IrGraph.h"

std::vector<std::size_t> IrGraph::GetHashesForState (const IrGraphState & ir_graph_state, ) const
{
    std::size_t last_hash = 0;
    std::vector<std::size_t> hashes_for_state;

    for (const auto & [cache_policy, _] : processors_)
    {
        last_hash = IrGraphHashing::GetHashForCachePolicy (cache_policy, last_hash, ir_graph_state);
        hashes_for_state.push_back (last_hash);
    }

    return hashes_for_state;
}