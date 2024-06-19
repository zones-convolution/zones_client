#pragma once

#include "format/IrData.h"
#include "format/ZoneMetadata.h"
#include "ir_engine/IrController.h"

class LoadController
{
public:
    explicit LoadController (juce::ThreadPool & thread_pool, IrController & ir_controller);
    void Load (const IrSelection & ir_selection, const std::function<void (bool)> & callback);
    void LoadFromDisk ();

    std::function<void ()> OnLoadingIrUpdated;
    const std::optional<IrSelection> & GetLoadingIr ();

private:
    void SetLoadingIr (const std::optional<IrSelection> & ir_selection);

    juce::ThreadPool & thread_pool_;
    IrController & ir_controller_;

    static const juce::String kFilePickerDialogTitle;
    std::unique_ptr<juce::FileChooser> file_picker_;
    std::vector<TargetFormat> valid_target_formats_;

    std::mutex loading_ir_mutex_;
    std::optional<IrSelection> loading_ir_;
};
