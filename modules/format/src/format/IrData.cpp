#include "IrData.h"

#include "IrFormatter.h"

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
        case ChannelFormat::kMono:
            return MonoFormatter::SupportsTarget (*this, target_format);
        case ChannelFormat::kStereo:
            return StereoFormatter::SupportsTarget (*this, target_format);
        case ChannelFormat::kFoa:
            return FoaFormatter::SupportsTarget (*this, target_format);
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

std::vector<TargetFormat> GetTargetFormatsForChannelSet (const juce::AudioChannelSet & channel_set)
{
    if (channel_set == juce::AudioChannelSet::mono ())
        return {TargetFormat::kMono};
    if (channel_set == juce::AudioChannelSet::stereo ())
        return {TargetFormat::kStereo, TargetFormat::kTrueStereo};
    if (channel_set == juce::AudioChannelSet::ambisonic (1))
        return {TargetFormat::kFoa};
    if (channel_set == juce::AudioChannelSet::quadraphonic ())
        return {TargetFormat::kFoa};

    return {};
}

std::string GetStringForTargetFormat (const TargetFormat & target_format)
{
    switch (target_format)
    {
        case TargetFormat::kMono:
            return "Mono";
        case TargetFormat::kStereo:
            return "Stereo";
        case TargetFormat::kTrueStereo:
            return "True Stereo";
        case TargetFormat::kFoa:
            return "FOA";
    }
}

std::optional<TargetFormat> GetTargetFormatForString (const std::string & string)
{
    if (string == "Mono")
        return TargetFormat::kMono;
    else if (string == "Stereo")
        return TargetFormat::kStereo;
    else if (string == "True Stereo")
        return TargetFormat::kTrueStereo;
    else if (string == "FOA")
        return TargetFormat::kFoa;
    else
        return std::nullopt;
}

int GetNumChannels (TargetFormat target_format)
{
    switch (target_format)
    {
        case TargetFormat::kMono:
            return 1;
        case TargetFormat::kStereo:
            return 2;
        case TargetFormat::kFoa:
            return 4;

        default:
            return 0;
    }
}

void CopyIrDataMeta (IrData & to, const IrData & from)
{
    to.bit_depth = from.bit_depth;
    to.sample_rate = from.sample_rate;
}
