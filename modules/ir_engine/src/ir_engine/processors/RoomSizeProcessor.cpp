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
    auto output_num_samples = juce::roundToInt (room_size * (float) input_num_samples);

    output_buffer.setSize (num_channels, output_num_samples);

    Stretch stretch;
    stretch.presetDefault (num_channels, state.sample_rate);

    stretch.process (input_buffer_unboxed.getArrayOfReadPointers (),
                     input_num_samples,
                     output_buffer.getArrayOfWritePointers (),
                     output_num_samples);
}
