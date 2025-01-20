#pragma once

#include "Preferences.h"
#include "ir_engine/IrController.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <rocket.hpp>

class PreferencesController
{
public:
    PreferencesController (IrController & ir_controller);

    void AddUserPath (std::function<void (Preferences)> add_path_callback);
    [[nodiscard]] Preferences RemoveUserPath (const std::filesystem::path & user_path) const;
    void RevealUserPath (const std::filesystem::path & user_path);
    [[nodiscard]] Preferences GetPreferences () const;

    struct VersionData
    {
        std::string version_number;
        std::string build_type;
    };

    void SetVersion (const VersionData & version_data);
    [[nodiscard]] VersionData GetVersion () const;

    struct BlockSizes
    {
        int maximum = 0;
        int current = 0;
    };

    rocket::signal<void ()> OnBlockSizeUpdated;
    void SetInternalBlockSize (const int new_blocksize);
    void SetMaxBlockSize (const int new_maximum);

    BlockSizes GetBlockSizes ();

private:
    static const juce::String kPathPickerDialogTitle;
    std::unique_ptr<juce::FileChooser> directory_picker_;

    IrController & ir_controller_;

    VersionData version_data_ {};

    int max_block_size_ = 0;
};
