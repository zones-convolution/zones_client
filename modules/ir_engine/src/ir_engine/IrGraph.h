#pragma once
#include "IrGraphHashing.h"
#include "IrGraphProcessor.h"
#include "processors/TestProcessor.h"

#include <array>
#include <juce_dsp/juce_dsp.h>
#include <string>

using ProcessorWithCachePolicy = std::pair<IrGraphCachePolicy &, IrGraphProcessor &>;

class IrGraph
{
public:
private:
    [[nodiscard]] std::vector<std::size_t>
    GetHashesForState (const IrGraphState & ir_graph_state) const;

    TestCachePolicy test_cache_policy_;
    TestProcessor test_processor_;
    std::array<ProcessorWithCachePolicy, 1> processors_ {
        ProcessorWithCachePolicy {test_cache_policy_, test_processor_}};
};