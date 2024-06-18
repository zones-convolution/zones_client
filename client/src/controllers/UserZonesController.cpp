#include "UserZonesController.h"

const juce::String UserZonesController::kChoosePickerDialogTitle = "Pick Audio File";

void UserZonesController::GetIrPath (
    std::function<void (const std::string & path)> add_ir_path_callback)
{
    file_picker_ = std::make_unique<juce::FileChooser> (
        kChoosePickerDialogTitle, juce::File (), "*.wav,*.aiff,*.mp3");
    auto picker_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    file_picker_->launchAsync (picker_flags,
                               [add_ir_path_callback] (const juce::FileChooser & chooser)
                               {
                                   auto path = chooser.getResult ();
                                   if (path.exists ())
                                       add_ir_path_callback (
                                           path.getFullPathName ().toStdString ());
                               });
}