#include "audio_engine/ConvolverUtilities.h"

#include <catch2/catch_test_macros.hpp>
#include <melatonin_test_helpers/melatonin_test_helpers.h>

SCENARIO ("Split Blocks can be cleared", "[SplitBlock]")
{
    SECTION ("can clear an empty block")
    {
        juce::dsp::AudioBlock<float> empty_block {};
        SplitBlock split_block {empty_block, empty_block};

        split_block.Clear ();
    }

    GIVEN ("a split block")
    {
        juce::AudioBuffer<float> buffer {1, 128};
        juce::dsp::AudioBlock<float> block {buffer};
        block.fill (1.0f);

        auto block_one = block.getSubBlock (0, 64);
        auto block_two = block.getSubBlock (64, 64);

        SplitBlock split_block {block_one, block_two};

        REQUIRE (melatonin::blockIsFilled (block));

        WHEN ("the block is cleared")
        {
            split_block.Clear ();
            THEN ("the full block is now empty")
            {
                REQUIRE (melatonin::blockIsEmpty (block));
            }
        }
    }
}