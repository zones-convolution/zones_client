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
                ir_reader.ReadIrData (load_path, *ir_format_data.position_map.centre, ir_data);
            }
            break;
        case TargetFormat::kStereo:
            if (ir_format_data.position_map.centre.has_value ())

            {
                IrReader ir_reader;

                IrData mono_ir_data;
                ir_reader.ReadIrData (load_path, *ir_format_data.position_map.centre, mono_ir_data);

                CopyIrDataMeta (ir_data, mono_ir_data);
                ir_data.buffer.setSize (2, mono_ir_data.buffer.getNumSamples ());
                juce::dsp::AudioBlock<float> ir_block {ir_data.buffer};
                ir_block.getSingleChannelBlock (0).copyFrom (
                    juce::dsp::AudioBlock<float> {mono_ir_data.buffer});
                ir_block.getSingleChannelBlock (1).copyFrom (
                    juce::dsp::AudioBlock<float> {mono_ir_data.buffer});
            }
            break;
        case TargetFormat::kQuadraphonic:
        case TargetFormat::kFoa:
            if (ir_format_data.position_map.centre.has_value ())

            {
                IrReader ir_reader;

                IrData mono_ir_data;
                ir_reader.ReadIrData (load_path, *ir_format_data.position_map.centre, mono_ir_data);

                CopyIrDataMeta (ir_data, mono_ir_data);

                auto num_ir_data_channels = 4;
                ir_data.buffer.setSize (num_ir_data_channels, mono_ir_data.buffer.getNumSamples ());
                juce::dsp::AudioBlock<float> ir_block {ir_data.buffer};
                for (auto channel_index = 0; channel_index < num_ir_data_channels; ++channel_index)
                {
                    ir_block.getSingleChannelBlock (channel_index)
                        .copyFrom (juce::dsp::AudioBlock<float> {mono_ir_data.buffer});
                }
            }
            break;
    }
}
