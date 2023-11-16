#include "OverlapSaveConvolver.h"

void OverlapSaveConvolver::prepare (const juce::dsp::ProcessSpec & spec)
{
    process_spec_ = spec;
}

void OverlapSaveConvolver::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    if (ir_forward_transform_.getNumChannels () == 0)
        return;

    auto input_block = replacing.getInputBlock ();
    auto output_block = replacing.getOutputBlock ();

    juce::dsp::AudioBlock<float> forward_transform_block (forward_transform_);
    juce::dsp::AudioBlock<float> ir_forward_transform_block (ir_forward_transform_);

    forward_transform_block.getSubBlock (num_samples_to_discard_).copyFrom (input_block);

    circular_buffer_.GetNext (0).CopyFrom (input_block);
    circular_buffer_.GetNext (input_block.getNumSamples ()).CopyTo (forward_transform_block);

    for (auto channel_index = 0; channel_index < input_block.getNumChannels (); ++channel_index)
    {
        auto channel = forward_transform_block.getChannelPointer (channel_index);
        auto ir_channel = ir_forward_transform_block.getChannelPointer (channel_index);

        fft_->performRealOnlyForwardTransform (channel, true);

        // multiply input fft with ir fft
        for (auto sample_index = 0; sample_index < forward_transform_block.getNumSamples ();
             sample_index += 2)
        {
            std::complex<float> x {channel [sample_index], channel [sample_index + 1]};
            std::complex<float> y {ir_channel [sample_index], ir_channel [sample_index + 1]};
            auto z = x * y;

            channel [sample_index] = z.real ();
            channel [sample_index + 1] = z.imag ();
        }

        // inverse fft result
        fft_->performRealOnlyInverseTransform (channel);
    }

    output_block.copyFrom (forward_transform_block.getSubBlock (num_samples_to_discard_));
    forward_transform_block.clear ();
}

void OverlapSaveConvolver::reset ()
{
}

void OverlapSaveConvolver::LoadImpulseResponse (juce::dsp::AudioBlock<float> ir_block,
                                                double sample_rate)
{
    auto ir_num_channels = ir_block.getNumChannels ();
    auto ir_num_samples = ir_block.getNumSamples ();             // M
    num_samples_to_discard_ = ir_num_samples - 1;                // M - 1
    auto max_block_num_samples = process_spec_.maximumBlockSize; // L
    fft_size_ = max_block_num_samples + num_samples_to_discard_; // N = L + M - 1
    fft_size_ = juce::nextPowerOfTwo (fft_size_);

    auto fft_order = std::log2 (fft_size_);
    fft_ = std::make_unique<juce::dsp::FFT> (fft_order);

    ir_forward_transform_.setSize (ir_num_channels, 2 * fft_size_, false);
    ir_forward_transform_.clear ();

    forward_transform_.setSize (process_spec_.numChannels, 2 * fft_size_, false);
    forward_transform_.clear ();

    saved_inputs_.setSize (
        process_spec_.numChannels, num_samples_to_discard_, false); // M - 1 samples long
    saved_inputs_.clear ();

    juce::dsp::AudioBlock<float> ir_forward_transform_block (ir_forward_transform_);
    ir_forward_transform_block.copyFrom (ir_block);

    for (auto channel_index = 0; channel_index < ir_num_channels; ++channel_index)
    {
        auto channel = ir_forward_transform_block.getChannelPointer (channel_index);
        fft_->performRealOnlyForwardTransform (channel, true);
    }
}
