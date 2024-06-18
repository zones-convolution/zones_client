#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class UserZonesController
{
public:
    UserZonesController () = default;

    void GetIrPath (std::function<void (const std::string & path)> add_ir_path_callback);

private:
    static const juce::String kChoosePickerDialogTitle;
    std::unique_ptr<juce::FileChooser> file_picker_;
};
