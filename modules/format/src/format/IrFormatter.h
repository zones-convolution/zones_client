#include "IrData.h"
#include "formatter/FoaFormatter.h"
#include "formatter/MonoFormatter.h"
#include "formatter/StereoFormatter.h"

#include <filesystem>
#include <juce_audio_basics/juce_audio_basics.h>

void CreateTargetIR (const std::filesystem::path & load_path,
                     const IrFormatData & ir_format_data,
                     TargetFormat target_format,
                     IrData & ir_data);