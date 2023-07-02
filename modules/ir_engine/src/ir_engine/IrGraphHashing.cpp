#include "IrGraphHashing.h"

std::size_t IrGraphHashing::CombineProcessorHashes (size_t previous_processor_hash,
                                                    size_t next_processor_hash)
{
    return previous_processor_hash ^ next_processor_hash;
}

std::size_t IrGraphHashing::GetHashForCachePolicy (const IrGraphCachePolicy & cache_policy,
                                                   size_t previous_processor_hash,
                                                   const IrGraphState & ir_graph_state)
{
    auto state_hash = cache_policy.GetHashForState (ir_graph_state);
    return CombineProcessorHashes (previous_processor_hash, state_hash);
}