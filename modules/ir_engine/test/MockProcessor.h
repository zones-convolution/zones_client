#pragma once
#include "ir_engine/IrGraphProcessor.h"

class MockProcessor : public IrGraphProcessor
{
public:
    void Process (BoxedBuffer & input_buffer,
                  juce::AudioBuffer<float> & output_buffer,
                  const IrGraphState & state) override
    {
        process_call_count += 1;
        last_input_buffer = input_buffer;

        const juce::dsp::AudioBlock<float> copy_block {buffer_to_copy_from};
        output_buffer.setSize (buffer_to_copy_from.getNumChannels (),
                               buffer_to_copy_from.getNumSamples ());
        auto output_block = juce::dsp::AudioBlock<float> (output_buffer);
        output_block.copyFrom (copy_block);
    }

    int process_call_count = 0;
    BoxedBuffer last_input_buffer;
    juce::AudioBuffer<float> buffer_to_copy_from;
};