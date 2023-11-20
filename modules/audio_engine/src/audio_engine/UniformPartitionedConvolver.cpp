#include "UniformPartitionedConvolver.h"

void UniformPartitionedConvolver::prepare (const juce::dsp::ProcessSpec & spec)
{
    process_spec_ = spec;
}

void UniformPartitionedConvolver::process (
    const juce::dsp::ProcessContextReplacing<float> & replacing)
{
}

void UniformPartitionedConvolver::reset ()
{
}

void UniformPartitionedConvolver::LoadImpulseResponse (juce::dsp::AudioBlock<float> ir_block,
                                                       double sample_rate)
{
    auto block_size = process_spec_.maximumBlockSize;
    fft_size_ = block_size * 2;
    auto fft_order = std::log2 (fft_size_);
    fft_ = std::make_unique<juce::dsp::FFT> (fft_order);

    auto num_samples = ir_block.getNumSamples ();
    auto num_partitions = std::ceil (num_samples / block_size);
    auto total_num_samples = num_partitions * block_size;
}
