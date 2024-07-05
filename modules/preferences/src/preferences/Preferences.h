#pragma once

#include <filesystem>
#include <juce_core/juce_core.h>
#include <nlohmann/json.hpp>
#include <vector>

struct Preferences
{
    void Save ();
    void Load ();

    static juce::File GetZonesDataDirectory ();
    static juce::File GetPreferencesFile ();

    std::vector<std::filesystem::path> user_paths {};
};

static void from_json (const nlohmann::json & data, Preferences & preferences)
{
    data.at ("userPaths").get_to (preferences.user_paths);
}

static void to_json (nlohmann::json & data, const Preferences & preferences)
{
    data = nlohmann::json {{"userPaths", preferences.user_paths}};
}