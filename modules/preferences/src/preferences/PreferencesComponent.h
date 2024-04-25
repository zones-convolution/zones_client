#pragma once

#include "Preferences.h"
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/DividerComponent.h"
#include "look_and_feel/components/IconButton.h"
#include "look_and_feel/components/IconTextButton.h"

#include <filesystem>
#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>

class UserPathComponent : public juce::Component
{
public:
    explicit UserPathComponent (const std::filesystem::path & user_path);
    ~UserPathComponent () override = default;

    void resized () override;

    std::function<void ()> OnPathRemoved;

private:
    std::filesystem::path user_path_;
    juce::Label path_label_;
    IconButton remove_path_button_ {"Remove Path", BoxIcons::kBxTrash};
    IconButton reveal_path_ {"Reveal Path", BoxIcons::kBxSushi};
};

class PreferencesComponent : public juce::Component
{
public:
    PreferencesComponent ();
    ~PreferencesComponent () override = default;

    void resized () override;

private:
    static const juce::String kPathPickerDialogTitle;

    void AddPath ();
    void UpdatePreferences ();

    Preferences preferences_;
    juce::Label preferences_label_ {"Preferences", "Preferences"};
    DividerComponent top_divider_;

    IconTextButton refresh_button_ {"Refresh", BoxIcons::kBxRefresh};
    juce::TextButton add_path_button_ {"Add Path"};

    std::unique_ptr<juce::FileChooser> directory_picker_;

    std::vector<std::unique_ptr<UserPathComponent>> user_path_components_;
};