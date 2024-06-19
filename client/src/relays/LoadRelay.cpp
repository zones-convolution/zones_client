#include "LoadRelay.h"

#include "format/ZoneMetadataJson.h"

LoadRelay::LoadRelay (juce::WebBrowserComponent & web_browser_component,
                      LoadController & load_controller)
    : web_browser_component_ (web_browser_component)
    , load_controller_ (load_controller)
{
}

juce::WebBrowserComponent::Options
LoadRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions.withNativeFunction (
        "load_zone_native",
        [&] (auto & var, auto complete)
        {
            IrSelection ir_selection;
            json::parse (var [0].toString ().toStdString ()).get_to (ir_selection);
            load_controller_.Load (ir_selection);
            complete ({});
        });
}
