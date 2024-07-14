#include "ir_engine/processors/RoomSizeProcessor.h"

#include <catch2/catch_test_macros.hpp>
#include <melatonin_test_helpers/melatonin_test_helpers.h>

static IrGraphProcessor::BoxedBuffer CreateInputBuffer (int num_channels, int num_samples)
{
    juce::AudioBuffer<float> input_buffer;
    input_buffer.setSize (num_channels, num_samples);

    for (auto j = 0; j < num_channels; ++j)
        for (int i = 0; i < num_samples; ++i)
            input_buffer.setSample (j, i, 0.5);

    return input_buffer;
}

TEST_CASE ("unchanged room size", "[IrEngine]")
{
    RoomSizeProcessor room_size_processor;

    auto num_samples = 100;
    auto num_channels = 1;

    auto input = CreateInputBuffer (num_channels, num_samples);
    juce::AudioBuffer<float> output_buffer;
    room_size_processor.Process (input, output_buffer, {.room_size = 1.0f});

    REQUIRE (output_buffer.getNumChannels () == num_channels);
    REQUIRE (output_buffer.getNumSamples () == num_samples);
}

TEST_CASE ("decrease room size", "[IrEngine]")
{
}

TEST_CASE ("increase room size", "[IrEngine]")
{
}