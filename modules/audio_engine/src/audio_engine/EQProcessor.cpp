
#include "EQProcessor.h"

void EQProcessor::prepare (const juce::dsp::ProcessSpec & spec)
{
    auto sample_rate = spec.sampleRate;
    spec_ = spec;

    UpdateFilters (1, 1);
    processor_chain_.prepare (spec);
}

void EQProcessor::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    processor_chain_.process (replacing);
}

void EQProcessor::reset ()
{
    processor_chain_.reset ();
}

void EQProcessor::UpdateFilters (float bass, float treble)
{
    *processor_chain_.get<0> ().state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf (
        spec_.sampleRate, low_cutoff, q_factor_, bass);

    *processor_chain_.get<1> ().state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf (
        spec_.sampleRate, high_cutoff, q_factor_, treble);
}
