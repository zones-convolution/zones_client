#include "OverlapAddConvolver.h"

void OverlapAddConvolver::prepare (const juce::dsp::ProcessSpec & spec)
{
    process_spec_ = spec;
}

void OverlapAddConvolver::process (const juce::dsp::ProcessContextReplacing<float> & replacing)
{
    // IF WE HAVE LOADED AN IR...

    auto input_block = replacing.getInputBlock ();
    auto output_block = replacing.getOutputBlock ();

    juce::dsp::AudioBlock<float> input_block_forward_transform (input_block_forward_transform_);
    input_block_forward_transform.clear ();
    input_block_forward_transform.copyFrom (input_block);

    juce::dsp::AudioBlock<float> ir_forward_transform_block (ir_forward_transform_);

    juce::dsp::AudioBlock<float> delay_block (delay_line_);

    for (auto channel_index = 0; channel_index < output_block.getNumChannels (); ++channel_index)
    {
        auto channel = input_block_forward_transform.getChannelPointer (channel_index);
        auto ir_channel = ir_forward_transform_block.getChannelPointer (channel_index);

        fft_->performRealOnlyForwardTransform (channel, true);

        // multiply input fft with ir fft
        for (auto sample_index = 0; sample_index < input_block_forward_transform.getNumSamples ();
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

        auto channel_block = input_block_forward_transform.getSingleChannelBlock (channel_index);
        auto delay_line_num_samples = delay_line_.getNumSamples ();

        auto output_block_num_samples = input_block.getNumSamples ();

        for (auto sample_index = 0; sample_index < fft_size_; ++sample_index)
        {
            auto num_samples_to_fill = delay_line_num_samples - read_position_;
            auto block_to_fill = delay_block.getSingleChannelBlock (channel_index)
                                     .getSubBlock (read_position_, num_samples_to_fill);

            auto remaining_samples = delay_line_num_samples - num_samples_to_fill;
            auto remaining_block = delay_block.getSingleChannelBlock (channel_index)
                                       .getSubBlock (0, remaining_samples);

            block_to_fill.add (channel_block.getSubBlock (0, num_samples_to_fill));
            remaining_block.add (
                channel_block.getSubBlock (num_samples_to_fill, remaining_samples));

            auto num_samples_to_take =
                std::min (static_cast<int> (output_block_num_samples), num_samples_to_fill);
            output_block.getSingleChannelBlock (channel_index)
                .copyFrom (channel_block.getSubBlock (read_position_, num_samples_to_take));

            auto remaining_samples_to_take = output_block_num_samples - num_samples_to_take;
            output_block.getSingleChannelBlock (channel_index)
                .getSubBlock (num_samples_to_take)
                .copyFrom (channel_block.getSubBlock (0, remaining_samples_to_take));

            read_position_ = (read_position_ + output_block_num_samples) % delay_line_num_samples;
        }
    }
}

void OverlapAddConvolver::reset ()
{
}

void OverlapAddConvolver::LoadImpulseResponse (juce::dsp::AudioBlock<float> ir_block,
                                               double sample_rate)
{
    int ir_num_channels = ir_block.getNumChannels ();
    int ir_num_samples = ir_block.getNumSamples ();                                // M
    int max_block_num_samples = process_spec_.maximumBlockSize;                    // N

    fft_size_ = juce::nextPowerOfTwo (ir_num_samples + max_block_num_samples - 1); // K

    auto fft_order = std::log2 (fft_size_);
    fft_ = std::make_unique<juce::dsp::FFT> (fft_order);

    ir_forward_transform_.setSize (ir_num_channels, 2 * fft_size_, false);
    ir_forward_transform_.clear ();

    input_block_forward_transform_.setSize (process_spec_.numChannels, 2 * fft_size_, false);
    input_block_forward_transform_.clear ();

    delay_line_.setSize (process_spec_.numChannels, fft_size_, false);
    delay_line_.clear ();

    juce::dsp::AudioBlock<float> ir_forward_transform_block (ir_forward_transform_);
    ir_forward_transform_block.copyFrom (ir_block);

    for (auto channel_index = 0; channel_index < ir_num_channels; ++channel_index)
    {
        auto channel = ir_forward_transform_block.getChannelPointer (channel_index);
        fft_->performRealOnlyForwardTransform (channel, true);
    }
}
