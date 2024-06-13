#pragma once

#include "Preferences.h"
#include "model/Action.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>

class PreferencesController
{
public:
    explicit PreferencesController (lager::context<Action> context);

    void AddUserPath ();
    void RemoveUserPath (const std::filesystem::path & user_path);
    void RevealUserPath (const std::filesystem::path & user_path);

    [[nodiscard]] const Preferences & GetPreferences () const;
    std::function<void ()> OnPreferencesUpdated;

private:
    static const juce::String kPathPickerDialogTitle;

    void UpdatePreferences ();

    lager::context<Action> context_;
    std::unique_ptr<juce::FileChooser> directory_picker_;
    Preferences preferences_;
};
