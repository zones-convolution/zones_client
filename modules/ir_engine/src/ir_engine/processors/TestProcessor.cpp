#include "TestProcessor.h"

std::size_t TestCachePolicy::GetHashForState (const IrGraphState & state) const
{
    return 0;
}

void TestProcessor::Process (juce::dsp::ProcessContextNonReplacing<float> & process_context)
{
}
