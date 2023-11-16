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
    ReadCircularBuffer ();
    WriteCircularBuffer (input_block);
    read_position_ =
        (read_position_ + input_block.getNumSamples ()) % circular_buffer_.getNumSamples ();

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

void OverlapSaveConvolver::ReadCircularBuffer ()
{
    juce::dsp::AudioBlock<float> circular_block {circular_buffer_};
    juce::dsp::AudioBlock<float> forward_transform_block {forward_transform_};

    auto circular_num_samples = circular_block.getNumSamples ();

    auto first_samples_to_take = circular_num_samples - read_position_;
    auto wrapped_samples_to_take = circular_num_samples - first_samples_to_take;

    auto first_block_to_take = circular_block.getSubBlock (read_position_, first_samples_to_take);
    auto wrapped_block_to_take = circular_block.getSubBlock (0, wrapped_samples_to_take);

    forward_transform_block.copyFrom (first_block_to_take);
    forward_transform_block.getSubBlock (first_samples_to_take).copyFrom (wrapped_block_to_take);
}

void OverlapSaveConvolver::WriteCircularBuffer (juce::dsp::AudioBlock<const float> input_block)
{
    juce::dsp::AudioBlock<float> circular_block {circular_buffer_};
    auto circular_num_samples = circular_block.getNumSamples ();
    auto input_block_num_samples = input_block.getNumSamples ();

    auto remaining_samples_before_wrap = circular_num_samples - read_position_;
    auto first_samples_to_fill = std::min (input_block_num_samples, remaining_samples_before_wrap);
    auto remaining_samples_to_fill = input_block_num_samples - first_samples_to_fill;

    auto first_block_to_fill = circular_block.getSubBlock (read_position_, first_samples_to_fill);
    auto remaining_block_to_fill = circular_block.getSubBlock (0, remaining_samples_to_fill);

    first_block_to_fill.copyFrom (input_block.getSubBlock (0, first_samples_to_fill));
    remaining_block_to_fill.copyFrom (
        input_block.getSubBlock (first_samples_to_fill, remaining_samples_to_fill));
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

    circular_buffer_.setSize (
        process_spec_.numChannels, num_samples_to_discard_, false); // M - 1 samples long
    circular_buffer_.clear ();

    juce::dsp::AudioBlock<float> ir_forward_transform_block (ir_forward_transform_);
    ir_forward_transform_block.copyFrom (ir_block);

    for (auto channel_index = 0; channel_index < ir_num_channels; ++channel_index)
    {
        auto channel = ir_forward_transform_block.getChannelPointer (channel_index);
        fft_->performRealOnlyForwardTransform (channel, true);
    }
}
