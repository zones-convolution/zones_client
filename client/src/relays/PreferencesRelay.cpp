#include "PreferencesRelay.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

PreferencesRelay::PreferencesRelay (juce::WebBrowserComponent & web_browser_component,
                                    PreferencesController & preferences_controller)
    : web_browser_component_ (web_browser_component)
    , preferences_controller_ (preferences_controller)
{
    preferences_controller.OnPreferencesUpdated = [&]
    {
        const auto & preferences = preferences_controller_.GetPreferences ();
        json data = preferences;
        web_browser_component_.emitEventIfBrowserIsVisible ("on_preferences_updated_native",
                                                            {data.dump ()});
    };
}

PreferencesRelay::~PreferencesRelay ()
{
    preferences_controller_.OnPreferencesUpdated = nullptr;
}

juce::WebBrowserComponent::Options
PreferencesRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions
        .withNativeFunction ("add_user_path_native",
                             [&] (auto & var, auto complete)
                             {
                                 preferences_controller_.AddUserPath ();
                                 complete ({});
                             })
        .withNativeFunction ("remove_user_path_native",
                             [&] (auto & var, auto complete)
                             {
                                 auto user_path = var [0].toString ().toStdString ();
                                 preferences_controller_.RemoveUserPath (user_path);
                                 complete ({});
                             })
        .withNativeFunction ("reveal_user_path_native",
                             [&] (auto & var, auto complete)
                             {
                                 auto user_path = var [0].toString ().toStdString ();
                                 preferences_controller_.RevealUserPath (user_path);
                                 complete ({});
                             })
        .withNativeFunction ("get_preferences_native",
                             [&] (auto & var, auto complete)
                             {
                                 const auto & preferences =
                                     preferences_controller_.GetPreferences ();
                                 json data = preferences;
                                 complete ({data.dump ()});
                             });
}