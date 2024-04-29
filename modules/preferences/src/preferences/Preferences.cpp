#include "Preferences.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

juce::File Preferences::GetZonesDataDirectory ()
{
    return juce::File::getSpecialLocation (
               juce::File::SpecialLocationType::userApplicationDataDirectory)
        .getChildFile ("zones");
}

juce::File Preferences::GetPreferencesFile ()
{
    return GetZonesDataDirectory ().getChildFile ("preferences.json");
}

static void from_json (const json & data, Preferences & preferences)
{
    data.at ("user_paths").get_to (preferences.user_paths);
}

static void to_json (json & data, const Preferences & preferences)
{
    data = json {{"user_paths", preferences.user_paths}};
}

void Preferences::Save ()
{
    auto preferences_file = GetPreferencesFile ();
    preferences_file.create ();

    json data = *this;
    std::ofstream stream (preferences_file.getFullPathName ().toStdString ());
    stream << std::setw (4) << data << std::endl;
}

void Preferences::Load ()
{
    auto preferences_file = GetPreferencesFile ();
    std::ifstream stream (preferences_file.getFullPathName ().toStdString ());
    json::parse (stream).get_to (*this);
}