#pragma once

#include "ZoneMetadata.h"

#include <filesystem>
#include <juce_audio_basics/juce_audio_basics.h>

enum class TargetFormat
{
    kMono,
    kStereo,
    kTrueStereo,
    kFoa,
    kQuadraphonic
};

bool IsTargetSupported (const juce::AudioChannelSet & channel_set, TargetFormat target_format);

std::vector<TargetFormat> GetTargetFormatsForChannelSet (const juce::AudioChannelSet & channel_set);

std::string GetStringForTargetFormat (const TargetFormat & target_format);
std::optional<TargetFormat> GetTargetFormatForString (const std::string & string);
int GetNumChannels (TargetFormat target_format);

struct IrSelection
{
    ZoneMetadata zone;
    IrMetadata ir;
    TargetFormat target_format;

    bool operator== (const IrSelection & rhs) const
    {
        return std::tie (zone, ir, target_format) == std::tie (rhs.zone, rhs.ir, rhs.target_format);
    }
};

struct IrFormatData
{
    ChannelFormat channel_format;
    PositionMap position_map;
    [[nodiscard]] bool SupportsTarget (TargetFormat target_format) const;
};

struct IrData
{
    juce::AudioBuffer<float> buffer;
    double sample_rate;
    double bit_depth;
};

void CopyIrDataMeta (IrData & to, const IrData & from);

class IrDataFormat
{
public:
    static std::filesystem::path
    GetMetadataFileNameForIdentifier (const std::string & ir_identifier);

private:
    static const std::string kMetadataExtension;
};