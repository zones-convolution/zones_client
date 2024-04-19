#pragma once

#include <filesystem>
#include <juce_audio_basics/juce_audio_basics.h>

enum class TargetFormat
{
    kMono,
    kStereo,
    kTrueStereo,
    kFoa
};

enum class ChannelFormat
{
    kMono,
    kStereo,
    kFoa,
};

bool IsTargetSupported (const juce::AudioChannelSet & channel_set, TargetFormat target_format);

struct PositionMap
{
    std::optional<std::string> centre;
    std::optional<std::string> left;
    std::optional<std::string> right;
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

class IrDataFormat
{
public:
    static std::filesystem::path
    GetMetadataFileNameForIdentifier (const std::string & ir_identifier);
    static std::filesystem::path
    GetImpulseResponseFileNameForIdentifier (const std::string & ir_identifier);

private:
    static const std::string kMetadataExtension;
    static const std::string kImpulseResponseExtension;
};