#pragma once
#include "IrGraphCachePolicy.h"
#include "IrGraphHashing.h"
#include "IrGraphProcessor.h"
#include "IrGraphStateKey.h"
#include "processors/TestProcessor.h"

#include <array>
#include <juce_dsp/juce_dsp.h>
#include <string>

using ProcessorWithCachePolicy = std::pair<IrGraphCachePolicy<IrGraphState> &, IrGraphProcessor &>;

class IrGraph
{
public:
    [[nodiscard]] std::vector<GraphStateKey> GetKeysForState (const IrGraphState & state) const;

private:
    TestProcessor test_processor_;
    IrGraphCachePolicy<IrGraphState> test_cache_policy_ =
        IrGraphCachePolicy<IrGraphState> ().WithPolicyIdentifier ("test_processor");

    std::array<ProcessorWithCachePolicy, 1> processors_ {
        ProcessorWithCachePolicy {test_cache_policy_, test_processor_}};
};