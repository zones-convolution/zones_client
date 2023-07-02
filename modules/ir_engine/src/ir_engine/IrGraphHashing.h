#pragma once
#include "IrGraphProcessor.h"

#include <vector>

class IrGraphHashing
{
public:
    [[nodiscard]] static std::size_t GetHashForCachePolicy (const IrGraphCachePolicy & cache_policy,
                                                            size_t previous_processor_hash,
                                                            const IrGraphState & ir_graph_state);

private:
    [[nodiscard]] static std::size_t CombineProcessorHashes (size_t previous_processor_hash,
                                                             size_t next_processor_hash);
};
