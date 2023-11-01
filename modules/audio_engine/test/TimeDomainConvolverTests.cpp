#include "audio_engine/TimeDomainConvolver.h"
#include "melatonin_test_helpers/melatonin_test_helpers.h"

#include <catch2/catch_test_macros.hpp>

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
            juce::AudioBuffer<float> buffer (kProcessSpec.numChannels,
                                             kProcessSpec.maximumBlockSize);
            juce::dsp::AudioBlock<float> block {buffer};
            block.fill (1.0f);

            THEN ("the output is unchanged")
            {
                REQUIRE_THAT (block, melatonin::isEqualTo<float> (block));
            }
        }
    }
}