#pragma once
#include <juce_dsp/juce_dsp.h>

class OverlapSaveConvolver : public juce::dsp::ProcessorBase
{
public:
    void prepare (const juce::dsp::ProcessSpec & spec) override;
    void process (const juce::dsp::ProcessContextReplacing<float> & replacing) override;
    void reset () override;
    void LoadImpulseResponse (juce::dsp::AudioBlock<float> ir_block, double sample_rate);

private:
    void ReadCircularBuffer ();
    void WriteCircularBuffer (juce::dsp::AudioBlock<const float> input_block);
    juce::dsp::ProcessSpec process_spec_;

    int read_position_ = 0;
    int num_samples_to_discard_;
    int fft_size_;
    std::unique_ptr<juce::dsp::FFT> fft_;

    juce::AudioBuffer<float> ir_forward_transform_;
    juce::AudioBuffer<float> forward_transform_;
    juce::AudioBuffer<float> circular_buffer_;
};
