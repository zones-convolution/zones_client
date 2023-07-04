#pragma once
#include "IrGraphStateKey.h"
#include "juce_core/juce_core.h"

#include <optional>
#include <unordered_map>

class ProcessResultPool
{
public:
    using SharedBuffer = std::shared_ptr<juce::AudioBuffer<float>>;

    std::optional<SharedBuffer> GetResult (const GraphStateKey & key);
    void CacheResult (const GraphStateKey & key, const SharedBuffer & buffer);
    void RemoveUnusedKeys (const std::vector<GraphStateKey> & used_keys);

private:
    std::mutex mutex_;

    std::unordered_map<GraphStateKey, SharedBuffer, GraphStateKeyHashFn> process_results_ {};
};
