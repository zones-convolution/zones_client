#include "FoaFormatter.h"

#include "../io/IrReader.h"

#include <juce_dsp/juce_dsp.h>

bool FoaFormatter::SupportsTarget (const IrFormatData & ir_format_data, TargetFormat target_format)
{
    switch (target_format)
    {
        case TargetFormat::kMono:
            return true;
        case TargetFormat::kFoa:
            return true;

        default:
            return false;
    }
}

void FoaFormatter::Format (const std::filesystem::path & load_path,
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

                juce::dsp::AudioBlock<float> ir_block {ir_data.buffer};
                ir_block.copyFrom (
                    juce::dsp::AudioBlock<float> {centre_position.buffer}.getSingleChannelBlock (
                        0));
            }

            // throw error

            break;
        case TargetFormat::kFoa:
            if (ir_format_data.position_map.centre.has_value ())
            {
                IrReader ir_reader;

                ir_reader.ReadIrData (load_path, *ir_format_data.position_map.centre, ir_data);
            }

            // throw error
            
            break;
    }
}