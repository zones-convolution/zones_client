#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_core/juce_core.h"
#include "juce_dsp/juce_dsp.h"
#include "juce_graphics/juce_graphics.h"

#include <immer/box.hpp>

struct Spectrogram
{
    using BoxedBuffer = immer::box<juce::AudioBuffer<float>>;
    using BoxedUint8Buffer = immer::box<juce::AudioBuffer<std::uint8_t>>;
    static BoxedUint8Buffer CreateNormalisedSpectrogramData (BoxedBuffer buffer,
                                                             int base_num_sample,
                                                             double base_sample_rate);
};