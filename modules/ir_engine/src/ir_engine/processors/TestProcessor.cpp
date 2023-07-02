#include "TestProcessor.h"

std::size_t TestCachePolicy::GetHashForState (const IrGraphState & state) const
{
    auto hash = std::hash<int> {}(state.param_2);
    hash = hash ^ std::hash<float> {}(state.param_1);
    return hash;
}

void TestProcessor::Process (juce::dsp::ProcessContextNonReplacing<float> & process_context,
                             const IrGraphState & state)
{
}
