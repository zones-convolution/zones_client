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
    auto input_num_samples = input_buffer->getNumSamples ();
    auto num_channels = input_buffer->getNumChannels ();

    Stretch stretch;
    stretch.presetDefault (num_channels, state.sample_rate);

    /**
     * Perform Seek
     */
    auto one_interval = stretch.intervalSamples ();
    auto block_length = stretch.blockSamples ();
    auto seek_samples = one_interval + block_length;

    juce::AudioBuffer<float> seek_buffer {num_channels, seek_samples};
    seek_buffer.clear ();
    stretch.seek (
        seek_buffer.getArrayOfReadPointers (), seek_samples, static_cast<double> (room_size));

    /**
     * Prepare output buffer
     */
    auto output_num_samples = juce::roundToInt ((float) input_num_samples * room_size);
    output_buffer.setSize (num_channels, output_num_samples, true, true, false);
    juce::dsp::AudioBlock<float> output_block {output_buffer};

    /**
     * Process Stretch
     */
    stretch.process (input_buffer->getArrayOfReadPointers (),
                     input_num_samples,
                     output_buffer.getArrayOfWritePointers (),
                     output_num_samples);

    /**
     * Flush Output
     */
    auto output_latency = stretch.outputLatency ();
    juce::AudioBuffer<float> flush_buffer {num_channels, output_latency};
    flush_buffer.clear ();
    stretch.flush (flush_buffer.getArrayOfWritePointers (), output_latency);
    juce::dsp::AudioBlock<float> flush_block {flush_buffer};

    //    std::cout << "In: ";
    //    for (auto i = 0; i < input_buffer->getNumSamples (); ++i)
    //        std::cout << input_buffer->getSample (0, i) << ",";
    std::cout << "\n"
              << "Out: ";
    for (auto i = 0; i < flush_buffer.getNumSamples (); ++i)
        std::cout << flush_buffer.getSample (0, i) << ",";

    /**
     * Stitch the buffers
     */
    if (output_num_samples > output_latency)
    {
        // Shift valid samples by output_latency
        output_block.copyFrom (output_block.getSubBlock (output_latency));

        // Copy flushed output to end
        output_block.getSubBlock (output_num_samples - output_latency).copyFrom (flush_block);
    }
    else
    {
        output_block.copyFrom (flush_block.getSubBlock (output_latency - output_num_samples));
    }
}
