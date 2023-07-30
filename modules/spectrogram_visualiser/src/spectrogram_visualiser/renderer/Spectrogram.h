#pragma once

#include <immer/box.hpp>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_graphics/juce_graphics.h>

class Spectrogram
{
public:
    static juce::Image CreateSpectrogram (const juce::dsp::AudioBlock<float> & audio_block);
    static immer::box<juce::AudioBuffer<float>>
    PerformFFT (const juce::dsp::AudioBlock<float> & audio_block);
};