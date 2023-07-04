#pragma once
#include "IrGraphCachePolicy.h"
#include "IrGraphProcessor.h"
#include "IrGraphStateKey.h"
#include "processors/TestProcessor.h"

#include <array>
#include <immer/flex_vector.hpp>
#include <juce_dsp/juce_dsp.h>
#include <string>
#include <vector>

class IrGraph
{
public:
    using CachePolicy = IrGraphCachePolicy<IrGraphState>;
    using ProcessorWithCachePolicy = std::pair<CachePolicy, std::weak_ptr<IrGraphProcessor>>;

    [[nodiscard]] std::vector<GraphStateKey> GetKeysForState (const IrGraphState & state) const;

    [[nodiscard]] IrGraph WithProcessor (const ProcessorWithCachePolicy & processor) const;

private:
    immer::flex_vector<ProcessorWithCachePolicy> processors_;
};