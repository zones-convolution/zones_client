#pragma once

#include "format/IrData.h"
#include "format/ZoneMetadata.h"
#include "ir_engine/IrController.h"

#include <rocket.hpp>

class LoadController
{
public:
    explicit LoadController (juce::ThreadPool & thread_pool, IrController & ir_controller);
    void Load (const IrSelection & ir_selection, const std::function<void (bool)> & callback);
    void LoadFromDisk ();

    rocket::signal<void (const ZoneMetadata & zone_metadata)> OnZoneMetadataUpdated;

    rocket::signal<void ()> OnLoadingIrUpdated;
    const std::optional<IrSelection> & GetLoadingIr ();

    rocket::signal<void ()> OnCurrentIrUpdated;
    const std::optional<IrSelection> & GetCurrentIr ();

    void UpdateValidTargetFormats (const std::vector<TargetFormat> & target_formats);
    const std::vector<TargetFormat> & GetValidTargetFormats () const;
    rocket::signal<void ()> OnValidTargetFormatsUpdated;

private:
    void SetLoadingIr (const std::optional<IrSelection> & ir_selection);
    void SetCurrentIr (const std::optional<IrSelection> & ir_selection);

    juce::ThreadPool & thread_pool_;
    IrController & ir_controller_;

    static const juce::String kFilePickerDialogTitle;
    std::unique_ptr<juce::FileChooser> file_picker_;

    std::vector<TargetFormat> valid_target_formats_;

    std::mutex loading_ir_mutex_;
    std::optional<IrSelection> loading_ir_;

    std::mutex current_ir_mutex_;
    std::optional<IrSelection> current_ir_;
};
