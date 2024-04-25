#pragma once

#include <filesystem>
#include <juce_core/juce_core.h>
#include <vector>

struct Preferences
{
    bool Save ();
    void Load ();

    static juce::File GetZonesDataDirectory ();
    static juce::File GetPreferencesFile ();

    std::vector<std::filesystem::path> user_paths;
};
