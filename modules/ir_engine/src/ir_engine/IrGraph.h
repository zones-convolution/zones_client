#pragma once
#include "IrGraphCachePolicy.h"
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
    TestProcessor test_processor_;
    IrGraphCachePolicy test_cache_policy_ =
        IrGraphCachePolicy ().WithPolicyIdentifier ("test_processor");

    std::array<ProcessorWithCachePolicy, 1> processors_ {
        ProcessorWithCachePolicy {test_cache_policy_, test_processor_}};
};