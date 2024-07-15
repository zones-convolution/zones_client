#include "ir_engine/processors/RoomSizeProcessor.h"

#include <catch2/catch_test_macros.hpp>
#include <juce_dsp/juce_dsp.h>
#include <melatonin_test_helpers/melatonin_test_helpers.h>

static IrGraphProcessor::BoxedBuffer CreateInputBuffer (int num_channels, int num_samples)
{
    juce::AudioBuffer<float> input_buffer;
    input_buffer.setSize (num_channels, num_samples);

    for (auto j = 0; j < num_channels; ++j)
    {
        for (int i = 0; i < num_samples; ++i)
        {
            //            input_buffer.setSample (
            //                j, i, static_cast<float> (std::sin (static_cast<float> (i) * 0.5f)));
            input_buffer.setSample (j, i, 0.5f);
        }

        input_buffer.setSample (j, 0, 0.01f);
        input_buffer.setSample (j, 1, 0.01f);
        input_buffer.setSample (j, 2, 0.01f);
        input_buffer.setSample (j, 3, 0.01f);
        input_buffer.setSample (j, 4, 0.01f);
        input_buffer.setSample (j, 5, 0.01f);
        input_buffer.setSample (j, num_samples - 1, 0.99f);
        input_buffer.setSample (j, num_samples - 2, 0.99f);
        input_buffer.setSample (j, num_samples - 3, 0.99f);
        input_buffer.setSample (j, num_samples - 4, 0.99f);
        input_buffer.setSample (j, num_samples - 5, 0.99f);
        input_buffer.setSample (j, num_samples - 6, 0.99f);
        input_buffer.setSample (j, num_samples - 7, 0.99f);
        input_buffer.setSample (j, num_samples - 8, 0.99f);
    }

    return input_buffer;
}

TEST_CASE ("unchanged room size", "[IrEngine]")
{
    //    RoomSizeProcessor room_size_processor;
    //
    //    auto num_samples = 20;
    //    auto num_channels = 2;
    //
    //    auto input = CreateInputBuffer (num_channels, num_samples);
    //    juce::AudioBuffer<float> output_buffer;
    //    room_size_processor.Process (input, output_buffer, {.sample_rate = 48000, .room_size
    //    = 1.0f});
    //
    //    REQUIRE (output_buffer.getNumChannels () == num_channels);
    //    REQUIRE (output_buffer.getNumSamples () == num_samples);
    //
    //    juce::dsp::AudioBlock<const float> input_block {*input};
    //    juce::dsp::AudioBlock<const float> output_block {output_buffer};
    //
    //    REQUIRE_THAT (output_block, melatonin::isEqualTo (input_block));
}

TEST_CASE ("decrease room size", "[IrEngine]")
{
    RoomSizeProcessor room_size_processor;

    auto num_samples = 20;
    auto num_channels = 2;

    auto input = CreateInputBuffer (num_channels, num_samples);
    juce::AudioBuffer<float> output_buffer;
    room_size_processor.Process (input, output_buffer, {.sample_rate = 48000, .room_size = 1.0f});

    //    REQUIRE (output_buffer.getNumChannels () == num_channels);
    //    REQUIRE (2 * output_buffer.getNumSamples () == num_samples);

    std::cout << "In: ";
    for (auto i = 0; i < input->getNumSamples (); ++i)
        std::cout << input->getSample (0, i) << ",";

    std::cout << "\n"
              << "Out: ";
    for (auto i = 0; i < output_buffer.getNumSamples (); ++i)
        std::cout << output_buffer.getSample (0, i) << ",";
}

TEST_CASE ("increase room size", "[IrEngine]")
{
    //    RoomSizeProcessor room_size_processor;
    //
    //    auto num_samples = 20;
    //    auto num_channels = 2;
    //
    //    auto input = CreateInputBuffer (num_channels, num_samples);
    //    juce::AudioBuffer<float> output_buffer;
    //    room_size_processor.Process (input, output_buffer, {.sample_rate = 48000, .room_size
    //    = 2.0f});
    //
    //    REQUIRE (output_buffer.getNumChannels () == num_channels);
    //    REQUIRE (output_buffer.getNumSamples () == 2 * num_samples);
}