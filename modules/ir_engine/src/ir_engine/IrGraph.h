#pragma once
#include "IrGraphCachePolicy.h"
#include "IrGraphHashing.h"
#include "IrGraphProcessor.h"
#include "IrGraphStateKey.h"
#include "processors/TestProcessor.h"

#include <array>
#include <immer/flex_vector.hpp>
#include <juce_dsp/juce_dsp.h>
#include <string>
#include <vector>

using ProcessorWithCachePolicy =
    std::pair<IrGraphCachePolicy<IrGraphState>, std::weak_ptr<IrGraphProcessor>>;

class IrGraph
{
public:
    [[nodiscard]] std::vector<GraphStateKey> GetKeysForState (const IrGraphState & state) const;

    [[nodiscard]] IrGraph WithProcessor (const ProcessorWithCachePolicy & processor) const;

private:
    immer::flex_vector<ProcessorWithCachePolicy> processors_;
};