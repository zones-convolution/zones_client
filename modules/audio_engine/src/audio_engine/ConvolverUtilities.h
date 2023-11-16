#pragma once
#include <juce_dsp/juce_dsp.h>

class SplitBlock
{
public:
    SplitBlock (juce::dsp::AudioBlock<float> first_block,
                juce::dsp::AudioBlock<float> wrapped_block);

    void CopyFrom (const juce::dsp::AudioBlock<float> block);
    void AddFrom (const juce::dsp::AudioBlock<float> block);

    void CopyTo (juce::dsp::AudioBlock<float> block);
    void Clear ();

    SplitBlock GetSubBlock (std::size_t num_samples);

private:
    juce::dsp::AudioBlock<float> first_block_;
    juce::dsp::AudioBlock<float> wrapped_block_;

    std::size_t total_num_samples_ = 0;
};

class CircularBuffer
{
public:
    explicit CircularBuffer (juce::dsp::AudioBlock<float> block);
    SplitBlock GetNext (std::size_t advancement);

private:
    std::size_t head_position_ = 0;
    juce::dsp::AudioBlock<float> block_;
};

// void processOLA (juce::dsp::AudioBlock<float> post_fft_block,
//                  juce::dsp::AudioBlock<float> output_block)
//{
//     auto num_samples = output_block.getNumSamples ();
//
//     juce::AudioBuffer<float> cb {};
//
//     CircularBuffer circular_buffer {cb};
//     auto split_view = circular_buffer.GetNext (0);
//
//     split_view.AddFrom (post_fft_block);
//
//     auto write_split_view = split_view.GetSubBlock (num_samples);
//     write_split_view.CopyTo (output_block);
//     write_split_view.Clear ();
//
//     circular_buffer.GetNext (num_samples);
// }
//
// void processOLS (juce::dsp::AudioBlock<float> input_block,
//                  juce::dsp::AudioBlock<float> pre_fft_block)
//{
//     auto num_samples = input_block.getNumSamples ();
//
//     juce::AudioBuffer<float> cb {};
//     CircularBuffer circular_buffer {cb};
//
//     auto split_view = circular_buffer.GetNext (0);
//     split_view.CopyFrom (input_block);
//
//     auto next_view = circular_buffer.GetNext (num_samples);
//     next_view.CopyTo (pre_fft_block);
// }