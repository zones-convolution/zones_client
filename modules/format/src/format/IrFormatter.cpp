#include "IrFormatter.h"

void CreateTargetIR (const std::filesystem::path & load_path,
                     const IrFormatData & ir_format_data,
                     TargetFormat target_format,
                     IrData & ir_data)
{
    switch (ir_format_data.channel_format)
    {
        case ChannelFormat::kMono:
            MonoFormatter::Format (load_path, ir_format_data, target_format, ir_data);
            break;
        case ChannelFormat::kStereo:
            StereoFormatter::Format (load_path, ir_format_data, target_format, ir_data);
            break;
        case ChannelFormat::kFoa:
            FoaFormatter::Format (load_path, ir_format_data, target_format, ir_data);
        case ChannelFormat::kQuadraphonic:
            QuadraphonicFormatter::Format (load_path, ir_format_data, target_format, ir_data);
            break;
    }
}
