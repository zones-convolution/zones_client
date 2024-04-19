#include "TestProcessor.h"

void TestProcessor::Process (BoxedBuffer & input_buffer,
                             juce::AudioBuffer<float> & output_buffer,
                             const IrGraphState & state)
{
    output_buffer.makeCopyOf (input_buffer.get ());
}