#include "PreferencesController.h"

const juce::String PreferencesController::kPathPickerDialogTitle = "Pick Project Directory";

PreferencesController::PreferencesController (IrController & ir_controller)
    : ir_controller_ (ir_controller)
{
}

Preferences PreferencesController::GetPreferences () const
{
    Preferences preferences;
    preferences.Load ();
    return preferences;
}

void PreferencesController::AddUserPath (std::function<void (Preferences)> add_path_callback)
{
    directory_picker_ = std::make_unique<juce::FileChooser> (kPathPickerDialogTitle);
    auto directory_flags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;

    directory_picker_->launchAsync (
        directory_flags,
        [&, add_path_callback] (const juce::FileChooser & chooser)
        {
            Preferences preferences;
            preferences.Load ();

            auto path = chooser.getResult ();
            if (path.exists ())
            {
                auto & user_paths = preferences.user_paths;
                user_paths.emplace_back (path.getFullPathName ().toStdString ());

                std::sort (user_paths.begin (), user_paths.end ());
                user_paths.erase (std::unique (user_paths.begin (), user_paths.end ()),
                                  user_paths.end ());

                preferences.Save ();
            }

            add_path_callback (preferences);
        });
}

Preferences PreferencesController::RemoveUserPath (const std::filesystem::path & user_path) const
{
    Preferences preferences;
    preferences.Load ();
    std::erase_if (preferences.user_paths, [&] (const auto & path) { return path == user_path; });
    preferences.Save ();
    return preferences;
}

void PreferencesController::RevealUserPath (const std::filesystem::path & user_path)
{
    Preferences preferences;
    preferences.Load ();
    auto & user_paths = preferences.user_paths;
    auto path_it = std::find (user_paths.begin (), user_paths.end (), user_path);
    if (path_it != user_paths.end ())
        juce::File (path_it->string ()).revealToUser ();
}

void PreferencesController::SetVersion (const PreferencesController::VersionData & version_data)
{
    version_data_ = version_data;
}

PreferencesController::VersionData PreferencesController::GetVersion () const
{
    return version_data_;
}

void PreferencesController::SetInternalBlockSize (const int new_blocksize)
{
    if (new_blocksize <= max_block_size_)
    {
        ir_controller_.internalBlockSizeValueChanged (new_blocksize);
    }
}

void PreferencesController::SetMaxBlockSize (const int new_maximum)
{
    max_block_size_ = new_maximum;

    if (ir_controller_.GetCurrentGraphState ().convolver_block_size > new_maximum)
        ir_controller_.internalBlockSizeValueChanged (new_maximum);
}
PreferencesController::BlockSizes PreferencesController::GetBlockSizes ()
{
    return BlockSizes {.maximum = max_block_size_,
                       .current = ir_controller_.GetCurrentGraphState ().convolver_block_size};
}
