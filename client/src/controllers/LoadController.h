#pragma once

#include "format/IrData.h"
#include "format/ZoneMetadata.h"
#include "ir_engine/IrController.h"

enum class IrLoadingState
{
    kSuccess,
    kFailure,
    kPending,
    kLoading
};

class LoadController
{
public:
    explicit LoadController (juce::ThreadPool & thread_pool, IrController & ir_controller);
    void Load (const IrSelection & ir_selection);
    void LoadFromDisk ();

private:
    juce::ThreadPool & thread_pool_;
    IrController & ir_controller_;

    static const juce::String kFilePickerDialogTitle;
    std::unique_ptr<juce::FileChooser> file_picker_;

    std::optional<IrSelection> current_ir_;
    std::vector<TargetFormat> valid_target_formats_;
    IrLoadingState loading_state_;
};
