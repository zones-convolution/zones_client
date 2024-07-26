#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_core/juce_core.h"
#include "juce_dsp/juce_dsp.h"
#include "juce_graphics/juce_graphics.h"

#include <immer/box.hpp>

struct Spectrogram
{
    using BoxedBuffer = immer::box<juce::AudioBuffer<float>>;
    static BoxedBuffer CreateNormalisedSpectrogramData (BoxedBuffer buffer);
};