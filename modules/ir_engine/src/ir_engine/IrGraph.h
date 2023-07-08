#pragma once
#include "IrGraphCachePolicy.h"
#include "IrGraphProcessor.h"
#include "IrGraphStateKey.h"
#include "ProcessResultPool.h"
#include "processors/TestProcessor.h"

#include <array>
#include <immer/flex_vector.hpp>
#include <juce_dsp/juce_dsp.h>
#include <string>
#include <vector>

class IrGraph
{
public:
    using AbortCallback = std::function<bool ()>;
    using CachePolicy = IrGraphCachePolicy<IrGraphState>;
    using ProcessorWithCachePolicy = std::pair<CachePolicy, std::weak_ptr<IrGraphProcessor>>;

    [[nodiscard]] std::vector<GraphStateKey> GetKeysForState (const IrGraphState & state) const;

    [[nodiscard]] IrGraph WithProcessor (const ProcessorWithCachePolicy & processor) const;
    std::optional<IrGraphProcessor::BoxedBuffer> Process (const IrGraphState & state,
                                                          ProcessResultPool & process_result_pool,
                                                          AbortCallback abort_callback);

private:
    immer::flex_vector<ProcessorWithCachePolicy> processors_;
};