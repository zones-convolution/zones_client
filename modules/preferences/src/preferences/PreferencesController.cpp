#include "PreferencesController.h"

const juce::String PreferencesController::kPathPickerDialogTitle = "Pick Project Directory";

PreferencesController::PreferencesController (lager::context<Action> context)
    : context_ (context)
{
    UpdatePreferences ();
}

void PreferencesController::AddUserPath ()
{
    directory_picker_ = std::make_unique<juce::FileChooser> (kPathPickerDialogTitle);
    auto directory_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;

    directory_picker_->launchAsync (
        directory_flags,
        [&] (const juce::FileChooser & chooser)
        {
            auto path = chooser.getResult ();
            if (path.exists ())
            {
                auto & user_paths = preferences_.user_paths;
                user_paths.emplace_back (path.getFullPathName ().toStdString ());

                std::sort (user_paths.begin (), user_paths.end ());
                user_paths.erase (std::unique (user_paths.begin (), user_paths.end ()),
                                  user_paths.end ());

                preferences_.Save ();
                UpdatePreferences ();
            }
        });
}

void PreferencesController::RevealUserPath (const std::filesystem::path & user_path)
{
    auto & user_paths = preferences_.user_paths;
    auto path_it = std::find (user_paths.begin (), user_paths.end (), user_path);
    if (path_it != user_paths.end ())
        juce::File (path_it->string ()).revealToUser ();
}

void PreferencesController::RemoveUserPath (const std::filesystem::path & user_path)
{
    std::erase_if (preferences_.user_paths, [&] (const auto & path) { return path == user_path; });
    preferences_.Save ();
    UpdatePreferences ();
}

const Preferences & PreferencesController::GetPreferences () const
{
    return preferences_;
}

void PreferencesController::UpdatePreferences ()
{
    preferences_.Load ();

    if (OnPreferencesUpdated)
        OnPreferencesUpdated ();

    context_.dispatch (RefreshUserZonesAction {});
}
