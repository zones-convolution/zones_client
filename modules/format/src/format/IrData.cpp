#include "IrData.h"

#include "formatter/StereoFormatter.h"

const std::string IrDataFormat::kMetadataExtension = ".json";

std::filesystem::path
IrDataFormat::GetMetadataFileNameForIdentifier (const std::string & ir_identifier)
{
    return ir_identifier + kMetadataExtension;
}

bool IrFormatData::SupportsTarget (TargetFormat target_format) const
{
    switch (channel_format)
    {
        case ChannelFormat::kStereo:
            return StereoFormatter::SupportsTarget (*this, target_format);
        default:
            return false;
    }
}

bool IsTargetSupported (const juce::AudioChannelSet & channel_set, TargetFormat target_format)
{
    switch (target_format)
    {
        case TargetFormat::kMono:
            return channel_set == juce::AudioChannelSet::mono ();
        case TargetFormat::kStereo:
        case TargetFormat::kTrueStereo:
            return channel_set == juce::AudioChannelSet::stereo ();
        case TargetFormat::kFoa:
            return channel_set == juce::AudioChannelSet::ambisonic (1);
    }
}

void CopyIrDataMeta (IrData & to, const IrData & from)
{
    to.bit_depth = from.bit_depth;
    to.sample_rate = from.sample_rate;
}