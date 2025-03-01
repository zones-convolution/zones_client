#include "PreferencesRelay.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

static void from_json (const json & data, PreferencesController::VersionData & version_data)
{
    data.at ("versionNumber").get_to (version_data.version_number);
    data.at ("buildType").get_to (version_data.build_type);
}

static void to_json (json & data, const PreferencesController::VersionData & version_data)
{
    data = json {{"versionNumber", version_data.version_number},
                 {"buildType", version_data.build_type}};
}

static void from_json (const json & data, PreferencesController::BlockSizes & block_sizes)
{
    data.at ("maximum").get_to (block_sizes.maximum);
    data.at ("current").get_to (block_sizes.current);
}

static void to_json (json & data, const PreferencesController::BlockSizes & block_sizes)
{
    data = json {{"maximum", block_sizes.maximum}, {"current", block_sizes.current}};
}

PreferencesRelay::PreferencesRelay (juce::WebBrowserComponent & web_browser_component,
                                    PreferencesController & preferences_controller)
    : web_browser_component_ (web_browser_component)
    , preferences_controller_ (preferences_controller)
{
    connections_ += {preferences_controller.OnBlockSizeUpdated.connect (
        [&]
        {
            auto state = preferences_controller.GetBlockSizes ();
            json data = state;

            JUCE_ASSERT_MESSAGE_THREAD;
            web_browser_component_.emitEventIfBrowserIsVisible ("on_block_size_update_native",
                                                                {data.dump ()});
        })};
}

juce::WebBrowserComponent::Options
PreferencesRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions
        .withNativeFunction ("add_user_path_native",
                             [&] (auto & var, auto complete)
                             {
                                 juce::WeakReference<PreferencesRelay> weak_self = this;

                                 preferences_controller_.AddUserPath (
                                     [complete, weak_self] (Preferences result)
                                     {
                                         json data = result;
                                         JUCE_ASSERT_MESSAGE_THREAD;

                                         if (weak_self.get ())
                                             complete ({data.dump ()});
                                     });
                             })
        .withNativeFunction ("remove_user_path_native",
                             [&] (auto & var, auto complete)
                             {
                                 auto user_path = var [0].toString ().toStdString ();
                                 json data = preferences_controller_.RemoveUserPath (user_path);

                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({data.dump ()});
                             })
        .withNativeFunction ("reveal_user_path_native",
                             [&] (auto & var, auto complete)
                             {
                                 auto user_path = var [0].toString ().toStdString ();
                                 preferences_controller_.RevealUserPath (user_path);

                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({});
                             })
        .withNativeFunction ("get_preferences_native",
                             [&] (auto & var, auto complete)
                             {
                                 const auto & preferences =
                                     preferences_controller_.GetPreferences ();
                                 json data = preferences;

                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({data.dump ()});
                             })

        .withNativeFunction ("get_block_sizes_native",
                             [&] (auto & var, auto complete)
                             {
                                 json data = preferences_controller_.GetBlockSizes ();

                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({data.dump ()});
                             })
        .withNativeFunction (
            "set_internal_block_size_native",
            [&] (auto & var, auto complete)
            {
                // the string and error checking might be unnecessary here
                // weak reference might be needed
                std::string block_size_string;
                json::parse (var [0].toString ().toStdString ()).get_to (block_size_string);

                auto block_size_int = 0;
                try
                {
                    block_size_int = std::stoi (block_size_string);
                }
                catch (...)
                {
                }

                if (block_size_int > 0)
                    preferences_controller_.SetInternalBlockSize (block_size_int);

                JUCE_ASSERT_MESSAGE_THREAD;
                complete ({});
            })
        .withNativeFunction ("get_version_data_native",
                             [&] (auto & var, auto complete)
                             {
                                 const auto & version_data = preferences_controller_.GetVersion ();
                                 json data = version_data;

                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({data.dump ()});
                             });
}