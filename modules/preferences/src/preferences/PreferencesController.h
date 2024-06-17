#pragma once

#include "Preferences.h"
#include "model/Action.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>

class PreferencesController
{
public:
    PreferencesController () = default;

    void AddUserPath (std::function<void (Preferences)> add_path_callback);
    [[nodiscard]] Preferences RemoveUserPath (const std::filesystem::path & user_path) const;
    void RevealUserPath (const std::filesystem::path & user_path);
    [[nodiscard]] Preferences GetPreferences () const;

private:
    static const juce::String kPathPickerDialogTitle;
    std::unique_ptr<juce::FileChooser> directory_picker_;
};
