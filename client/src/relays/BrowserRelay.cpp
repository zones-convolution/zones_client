#include "BrowserRelay.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

juce::WebBrowserComponent::Options
BrowserRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions.withNativeFunction (
        "open_external_url_native",
        [&] (auto & var, auto complete)
        {
            try
            {
                std::string url_string;
                json::parse (var [0].toString ().toStdString ()).get_to (url_string);

                const auto opened = juce::URL (url_string).launchInDefaultBrowser ();

                JUCE_ASSERT_MESSAGE_THREAD;
                complete ({opened});
            }
            catch (...)
            {
                JUCE_ASSERT_MESSAGE_THREAD;
                complete ({false});
            }
        });
}
