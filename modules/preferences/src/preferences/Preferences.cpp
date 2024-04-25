#include "Preferences.h"

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

bool Preferences::Save ()
{
    juce::DynamicObject dynamic_object;

    juce::Array<juce::var> user_paths_var;
    for (auto & user_path : user_paths)
        user_paths_var.add (juce::var (user_path.string ()));

    dynamic_object.setProperty ("user_paths", user_paths_var);

    try
    {
        auto preferences_file = GetPreferencesFile ();
        preferences_file.create ();
        auto output_stream = preferences_file.createOutputStream ();
        output_stream->setPosition (0);
        output_stream->truncate ();

        dynamic_object.writeAsJSON (*output_stream, {});
        return true;
    }
    catch (...)
    {
        return false;
    }
}

static juce::DynamicObject ReadJsonFileToDynamic (const juce::File & json_file)
{
    auto metadata_string = json_file.loadFileAsString ();
    auto metadata_var = juce::JSON::parse (metadata_string);

    auto dynamic = metadata_var.getDynamicObject ();
    if (dynamic != nullptr)
        return *dynamic;
    else
        return juce::DynamicObject {};
}

void Preferences::Load ()
{
    auto preferences_file = GetPreferencesFile ();
    auto dynamic_object = ReadJsonFileToDynamic (preferences_file);

    user_paths.clear ();
    const auto & user_paths_var = dynamic_object.getProperty ("user_paths");
    if (user_paths_var.isArray ())
    {
        auto & user_paths_array = *user_paths_var.getArray ();
        for (auto & user_path : user_paths_array)
        {
            if (user_path.isString ())
                user_paths.emplace_back (user_path.toString ().toStdString ());
        }
    }
}
