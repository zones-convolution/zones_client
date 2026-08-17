#include "MonoFormatter.h"

#include "../io/IrReader.h"

#include <juce_dsp/juce_dsp.h>

bool MonoFormatter::SupportsTarget (const IrFormatData & ir_format_data, TargetFormat target_format)
{
    switch (target_format)
    {
        case TargetFormat::kMono:
        case TargetFormat::kStereo:
        case TargetFormat::kQuadraphonic:
        case TargetFormat::kFoa:
            return true;

        default:
            return false;
    }
}

void MonoFormatter::Format (const std::filesystem::path & load_path,
                            const IrFormatData & ir_format_data,
                            TargetFormat target_format,
                            IrData & ir_data)
{
    switch (target_format)
    {
        case TargetFormat::kMono:
            if (ir_format_data.position_map.centre.has_value ())

            {
                IrReader ir_reader;

                IrData centre_position;
                ir_reader.ReadIrData (
                    load_path, *ir_format_data.position_map.centre, centre_position);

                auto num_samples = centre_position.buffer.getNumSamples ();
                ir_data.buffer.setSize (1, num_samples);
                ir_data.buffer.clear ();

                juce::dsp::AudioBlock<float> ir_block {ir_data.buffer};
                juce::dsp::AudioBlock<float> centre_block {centre_position.buffer};

                ir_block.copyFrom (centre_block);

                CopyIrDataMeta (ir_data, centre_position);
            }
            break;
        case TargetFormat::kStereo:
            if (ir_format_data.position_map.centre.has_value ())

            {
                IrReader ir_reader;

                IrData centre_position;
                ir_reader.ReadIrData (
                    load_path, *ir_format_data.position_map.centre, centre_position);

ir_data.buffer.setSize (2, centre_position.buffer.getNumSamples ());
                ir_data.buffer.clear ();

                juce::dsp::AudioBlock<float> ir_block {ir_data.buffer};
                ir_block.getSingleChannelBlock (0).copyFrom (
                    juce::dsp::AudioBlock<float> {centre_position.buffer});
                ir_block.getSingleChannelBlock (1).copyFrom (
                    juce::dsp::AudioBlock<float> {centre_position.buffer});

                CopyIrDataMeta (ir_data, centre_position);
            }
            break;
        case TargetFormat::kQuadraphonic:
        case TargetFormat::kFoa:
            if (ir_format_data.position_map.centre.has_value ())

            {
                IrReader ir_reader;

                IrData centre_position;
                ir_reader.ReadIrData (
                    load_path, *ir_format_data.position_map.centre, centre_position);

                auto num_ir_data_channels = 4;
                ir_data.buffer.setSize (num_ir_data_channels,
                                        centre_position.buffer.getNumSamples ());
                ir_data.buffer.clear ();

                juce::dsp::AudioBlock<float> ir_block {ir_data.buffer};
                for (auto channel_index = 0; channel_index < num_ir_data_channels; ++channel_index)
                {
                    ir_block.getSingleChannelBlock (channel_index)
                        .copyFrom (juce::dsp::AudioBlock<float> {centre_position.buffer});
                }

                CopyIrDataMeta (ir_data, centre_position);
            }
            break;
    }
}
