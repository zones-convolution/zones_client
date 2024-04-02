#include "ResamplerProcessor.h"

void ResamplerProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                                  juce::AudioBuffer<float> & output_buffer,
                                  const IrGraphState & state)
{
    auto buf = input_buffer;
    juce::dsp::AudioBlock<const float> input_block {*buf};
    output_buffer.setSize (input_buffer->getNumChannels (), input_buffer->getNumSamples ());
    juce::dsp::AudioBlock<float> output_block {output_buffer};
    output_block.copyFrom (input_block);
}
