#pragma once

#include "../IrData.h"

#include <filesystem>
#include <juce_audio_basics/juce_audio_basics.h>

namespace FoaFormatter
{
bool SupportsTarget (const IrFormatData & ir_format_data, TargetFormat target_format);
void Format (const std::filesystem::path & load_path,
             const IrFormatData & ir_format_data,
             TargetFormat target_format,
             IrData & ir_data);
}