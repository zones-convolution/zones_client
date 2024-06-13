#include "Preferences.h"

#include <fstream>

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