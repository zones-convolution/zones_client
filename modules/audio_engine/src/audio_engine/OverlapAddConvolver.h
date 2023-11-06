#pragma once
#include <juce_dsp/juce_dsp.h>

class OverlapAddConvolver : public juce::dsp::ProcessorBase
{
public:
    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;
    void LoadImpulseResponse (juce::dsp::AudioBlock<float> ir_block, double sample_rate);

private:
    void WriteDelayBlock ();
    void ReadDelayBlock (juce::dsp::AudioBlock<float> output_block);

    int read_position_ = 0;
    int fft_size_ = 0;
    juce::AudioBuffer<float> delay_line_;
    juce::dsp::ProcessSpec process_spec_;

    std::unique_ptr<juce::dsp::FFT> fft_;
    juce::AudioBuffer<float> ir_forward_transform_;
    juce::AudioBuffer<float> input_block_forward_transform_;
};
