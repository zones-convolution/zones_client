#include "RoomSizeProcessor.h"

#include "signalsmith-stretch.h"

using Stretch = signalsmith::stretch::SignalsmithStretch<float>;

void RoomSizeProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                                 juce::AudioBuffer<float> & output_buffer,
                                 const IrGraphState & state)
{
    /**
     * Processing an IR with a room size of zero will result in a 0 sample IR.
     */
    jassert (state.room_size > 0);

    auto room_size = state.room_size;
    auto & input_buffer_unboxed = input_buffer.get ();
    auto input_num_samples = input_buffer_unboxed.getNumSamples ();
    auto num_channels = input_buffer_unboxed.getNumChannels ();

    Stretch stretch;
    stretch.presetDefault (num_channels, state.sample_rate);

    auto input_latency = stretch.inputLatency ();
    auto output_latency = stretch.outputLatency ();

    auto resized_input_num_samples = input_num_samples + input_latency + output_latency;
    juce::AudioBuffer<float> resized_input {num_channels, resized_input_num_samples};
    juce::dsp::AudioBlock<float> resized_block {resized_input};
    resized_block.copyFrom (*input_buffer);

    // ========================

    auto actual_output_size = juce::roundToInt ((float) input_num_samples * room_size);

    auto output_num_samples = input_latency + output_latency + actual_output_size;

    output_buffer.setSize (num_channels, output_num_samples);

    stretch.process (resized_input.getArrayOfReadPointers (),
                     resized_input_num_samples,
                     output_buffer.getArrayOfWritePointers (),
                     output_num_samples);

    auto samples_to_remove = input_latency + output_latency;

    for (auto channel_index = 0; channel_index < num_channels; ++channel_index)
    {
        auto channel_ptr = output_buffer.getWritePointer (channel_index);
        for (auto sample_index = 0; sample_index < actual_output_size; ++sample_index)
        {
            channel_ptr [sample_index] = channel_ptr [sample_index + samples_to_remove];
        }
    }

    output_buffer.setSize (num_channels, actual_output_size, true, true, false);
}
