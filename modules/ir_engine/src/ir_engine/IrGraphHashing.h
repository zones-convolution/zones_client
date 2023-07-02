#pragma once
#include "IrGraphProcessor.h"

#include <vector>

class IrGraphHashing
{
public:
    [[nodiscard]] static std::size_t GetHashForCachePolicy (const IrGraphCachePolicy * cache_policy,
                                                            size_t previous_processor_hash,
                                                            const IrGraphState & ir_graph_state);

    [[nodiscard]] static std::vector<std::size_t>
    GetHashesForState (const IrGraphState & ir_graph_state,
                       const std::vector<IrGraphCachePolicy *> & cache_policies);

    [[nodiscard]] static std::size_t CombineHashes (size_t lhs, size_t rhs);
};
