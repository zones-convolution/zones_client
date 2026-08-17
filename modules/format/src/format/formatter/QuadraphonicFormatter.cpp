#include "QuadraphonicFormatter.h"

#include "../io/IrReader.h"

#include <juce_dsp/juce_dsp.h>

bool QuadraphonicFormatter::SupportsTarget (const IrFormatData & ir_format_data,
                                            TargetFormat target_format)
{
    switch (target_format)
    {
        case TargetFormat::kMono:
            return true;
        case TargetFormat::kQuadraphonic:
            return true;

        default:
            return false;
    }
}

void QuadraphonicFormatter::Format (const std::filesystem::path & load_path,
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

                auto num_centre_channels =
                    std::min (static_cast<int> (centre_block.getNumChannels ()), 4);

                for (auto channel_index = 0; channel_index < num_centre_channels; ++channel_index)
                    ir_block.add (centre_block.getSingleChannelBlock (channel_index));

                CopyIrDataMeta (ir_data, centre_position);
            }

            // throw error

            break;
        case TargetFormat::kQuadraphonic:
            if (ir_format_data.position_map.centre.has_value ())
            {
                IrReader ir_reader;

                IrData centre_position;
                ir_reader.ReadIrData (
                    load_path, *ir_format_data.position_map.centre, centre_position);

                auto num_samples = centre_position.buffer.getNumSamples ();
                ir_data.buffer.setSize (4, num_samples);
                ir_data.buffer.clear ();

                juce::dsp::AudioBlock<float> ir_block {ir_data.buffer};
                juce::dsp::AudioBlock<float> centre_block {centre_position.buffer};

                ir_block.copyFrom (centre_block);
                CopyIrDataMeta (ir_data, centre_position);
            }

            // throw error

            break;
    }
}