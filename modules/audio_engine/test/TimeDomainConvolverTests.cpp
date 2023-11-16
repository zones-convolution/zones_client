#include "audio_engine/TimeDomainConvolver.h"

#include <catch2/catch_test_macros.hpp>
#include <melatonin_test_helpers/melatonin_test_helpers.h>

static const juce::dsp::ProcessSpec kProcessSpec {.numChannels = 1,
                                                  .maximumBlockSize = 32,
                                                  .sampleRate = 48000};

SCENARIO ("Time Domain Convolution", "[TimeDomainConvolver]")
{
    GIVEN ("no impulse response is loaded")
    {
        TimeDomainConvolver convolver;
        convolver.prepare (kProcessSpec);

        WHEN ("process is called")
        {
            juce::AudioBuffer<float> input_buffer (kProcessSpec.numChannels,
                                                   kProcessSpec.maximumBlockSize);
            juce::dsp::AudioBlock<float> input_block {input_buffer};
            input_block.fill (1.f);

            juce::AudioBuffer<float> output_buffer (input_buffer);
            juce::dsp::AudioBlock<float> output_block {output_buffer};

            convolver.process ({output_block});
            THEN ("the output is unchanged")
            {
                REQUIRE_THAT (input_block, melatonin::isEqualTo<float> (output_block));
            }
        }
    }

    GIVEN ("a zeroed impulse response is loaded")
    {
        TimeDomainConvolver convolver;
        convolver.prepare (kProcessSpec);

        juce::AudioBuffer<float> ir_buffer (kProcessSpec.numChannels, 1);
        juce::dsp::AudioBlock<float> ir_block (ir_buffer);
        ir_block.fill (0.f);

        convolver.LoadImpulseResponse (ir_block, kProcessSpec.sampleRate);

        WHEN ("process is called")
        {
            juce::AudioBuffer<float> input_buffer (kProcessSpec.numChannels,
                                                   kProcessSpec.maximumBlockSize);
            juce::dsp::AudioBlock<float> input_block {input_buffer};
            input_block.fill (1.f);

            juce::AudioBuffer<float> output_buffer (input_buffer);
            juce::dsp::AudioBlock<float> output_block {output_buffer};

            convolver.process ({output_block});
            THEN ("the output is empty")
            {
                REQUIRE_THAT (output_block, melatonin::isEmpty ());
            }
        }
    }

    GIVEN ("a unity gain IR is loaded")
    {
        TimeDomainConvolver convolver;
        convolver.prepare (kProcessSpec);

        juce::AudioBuffer<float> ir_buffer (kProcessSpec.numChannels, 1);
        juce::dsp::AudioBlock<float> ir_block (ir_buffer);
        ir_block.fill (1.f);

        convolver.LoadImpulseResponse (ir_block, kProcessSpec.sampleRate);

        WHEN ("process is called")
        {
            juce::AudioBuffer<float> input_buffer (kProcessSpec.numChannels,
                                                   kProcessSpec.maximumBlockSize);
            juce::dsp::AudioBlock<float> input_block {input_buffer};
            input_block.fill (1.f);

            juce::AudioBuffer<float> output_buffer (input_buffer);
            juce::dsp::AudioBlock<float> output_block {output_buffer};

            convolver.process ({output_block});
            THEN ("the output is unchanged")
            {
                REQUIRE_THAT (input_block, melatonin::isEqualTo<float> (output_block));
            }
        }
    }

    GIVEN ("a decaying impulse response")
    {
        TimeDomainConvolver convolver;
        convolver.prepare (kProcessSpec);

        juce::AudioBuffer<float> ir_buffer (kProcessSpec.numChannels, 3);
        auto ir_block = juce::dsp::AudioBlock<float> (ir_buffer);
        auto ir_channel = ir_block.getChannelPointer (0);
        ir_channel [0] = 1.f;
        ir_channel [1] = .6f;
        ir_channel [2] = .2f;

        convolver.LoadImpulseResponse (ir_block, kProcessSpec.sampleRate);

        WHEN ("process is called")
        {
            juce::AudioBuffer<float> input_buffer (kProcessSpec.numChannels, 36);
            juce::dsp::AudioBlock<float> input_block {input_buffer};
            input_block.getSubBlock (0, 12).fill (1.f);
            input_block.getSubBlock (12, 12).fill (1.f);
            input_block.getSubBlock (24, 12).fill (0.f);

            juce::AudioBuffer<float> output_buffer (input_buffer);
            juce::dsp::AudioBlock<float> output_block {output_buffer};

            auto b1 = output_block.getSubBlock (0, 12);
            auto b2 = output_block.getSubBlock (12, 12);
            auto b3 = output_block.getSubBlock (24, 12);

            convolver.process (b1);
            convolver.process (b2);
            convolver.process (b3);

            THEN ("the output is convolved correctly")
            {
                std::array<float, 36> c1 {1.f,  1.6f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f,
                                          1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f,
                                          1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 1.8f, 0.8f, 0.2f, 0.f,
                                          0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f};

                float * convolved_result [1] = {c1.data ()};
                auto convolved_block = juce::dsp::AudioBlock<float> {convolved_result, 1, 36};
                REQUIRE_THAT (output_block, melatonin::isEqualTo<float> (convolved_block));
            }
        }
    }
}
