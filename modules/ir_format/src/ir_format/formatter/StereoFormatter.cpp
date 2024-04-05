#include "StereoFormatter.h"

#include "../io/IrReader.h"

#include <juce_dsp/juce_dsp.h>

bool StereoFormatter::SupportsTarget (const IrFormatData & ir_format_data,
                                      TargetFormat target_format)
{
    switch (target_format)
    {
        case TargetFormat::kMono:
            return true;
        case TargetFormat::kStereo:
            return true;
        case TargetFormat::kTrueStereo:
            return ir_format_data.position_map.left.has_value () &&
                   ir_format_data.position_map.right.has_value ();
        default:
            return false;
    }
}

void StereoFormatter::Format (const std::filesystem::path & load_path,
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
                juce::dsp::AudioBlock<float> centre_block {centre_position.buffer};

                ir_block.copyFrom (centre_block.getSingleChannelBlock (0));
                ir_block.add (centre_block.getSingleChannelBlock (1));
                ir_block.multiplyBy (0.5f);
            }
            else if (ir_format_data.position_map.right.has_value () &&
                     ir_format_data.position_map.left.has_value ())
            {
                IrReader ir_reader;

                IrData left_position;
                ir_reader.ReadIrData (load_path, *ir_format_data.position_map.left, left_position);

                IrData right_position;
                ir_reader.ReadIrData (
                    load_path, *ir_format_data.position_map.right, right_position);

                ir_data.buffer.setSize (1, left_position.buffer.getNumSamples ());
                juce::dsp::AudioBlock<float> ir_block {ir_data.buffer};
                ir_block.copyFrom (
                    juce::dsp::AudioBlock<float> {left_position.buffer}.getSingleChannelBlock (0));
                ir_block.add (
                    juce::dsp::AudioBlock<float> {right_position.buffer}.getSingleChannelBlock (1));
                ir_block.multiplyBy (0.5f);
            }

            // Throw error...

            break;
        case TargetFormat::kStereo:
            if (ir_format_data.position_map.centre.has_value ())
            {
                IrReader ir_reader;
                ir_reader.ReadIrData (load_path, *ir_format_data.position_map.centre, ir_data);
            }
            else if (ir_format_data.position_map.right.has_value () &&
                     ir_format_data.position_map.left.has_value ())
            {
                IrReader ir_reader;

                IrData left_position;
                ir_reader.ReadIrData (load_path, *ir_format_data.position_map.left, left_position);

                IrData right_position;
                ir_reader.ReadIrData (
                    load_path, *ir_format_data.position_map.right, right_position);

                ir_data.buffer.makeCopyOf (left_position.buffer);

                juce::dsp::AudioBlock<float> ir_block {ir_data.buffer};
                ir_block.add (juce::dsp::AudioBlock<float> {right_position.buffer});
                ir_block.multiplyBy (0.5f);
            }

            // Throw error...

            break;
        case TargetFormat::kTrueStereo:
            if (ir_format_data.position_map.right.has_value () &&
                ir_format_data.position_map.left.has_value ())
            {
                IrReader ir_reader;

                IrData left_position;
                ir_reader.ReadIrData (load_path, *ir_format_data.position_map.left, left_position);

                IrData right_position;
                ir_reader.ReadIrData (
                    load_path, *ir_format_data.position_map.right, right_position);

                ir_data.buffer.setSize (4, left_position.buffer.getNumSamples ());
                juce::dsp::AudioBlock<float> ir_block {ir_data.buffer};
                ir_block.copyFrom (juce::dsp::AudioBlock<float> {left_position.buffer});
                ir_block.getSubsetChannelBlock (2, 2).copyFrom (
                    juce::dsp::AudioBlock<float> {right_position.buffer});
            }

            // Throw error...

            break;
    }
}
