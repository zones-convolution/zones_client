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

    //    auto room_size = state.room_size;
    auto room_size = 1.f;
    auto & input_buffer_unboxed = input_buffer.get ();
    auto input_num_samples = input_buffer_unboxed.getNumSamples ();
    auto num_channels = input_buffer_unboxed.getNumChannels ();
    //
    auto output_num_samples = juce::roundToInt (room_size * (float) input_num_samples);

    output_buffer.setSize (num_channels, output_num_samples);

    Stretch stretch;
    stretch.presetDefault (num_channels, state.sample_rate);

    auto input_latency = stretch.inputLatency ();
    auto output_latency = stretch.outputLatency ();

    stretch.process (input_buffer_unboxed.getArrayOfReadPointers (),
                     input_num_samples,
                     output_buffer.getArrayOfWritePointers (),
                     output_num_samples);
}

// Currently using Signal Smith Stretch to stretch a fixed length
//     sound.I am noticing this is adding some delay to the output stretched signal.Reading
//         through the documentation,
//     there are some notes about use in non -
//         realtime.I am struggling to make sense of this.The way I have understood this is
//             that ```.inputLatency () +
//             .outputLatency ()``` samples should be added to the end of the input buffer. If I had
//             an