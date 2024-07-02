#pragma once
#include "IrGraphProcessor.h"
#include "IrGraphStateKey.h"
#include "juce_core/juce_core.h"

#include <optional>
#include <unordered_map>

class ProcessResultPool
{
public:
    std::optional<IrGraphProcessor::BoxedBuffer> GetResult (const GraphStateKey & key);
    void CacheResult (const GraphStateKey & key, const IrGraphProcessor::BoxedBuffer & buffer);
    void RemoveUnusedKeys (const std::vector<GraphStateKey> & used_keys);
    [[nodiscard]] int GetPoolSize ();

private:
    std::mutex mutex_;

    std::unordered_map<GraphStateKey, IrGraphProcessor::BoxedBuffer, GraphStateKeyHashFn>
        process_results_ {};
};
