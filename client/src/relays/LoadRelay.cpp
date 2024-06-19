#include "LoadRelay.h"

#include "format/ZoneMetadataJson.h"

LoadRelay::LoadRelay (juce::WebBrowserComponent & web_browser_component,
                      LoadController & load_controller)
    : web_browser_component_ (web_browser_component)
    , load_controller_ (load_controller)
{
    load_controller_.OnLoadingIrUpdated = [&]
    {
        auto loading_ir = load_controller_.GetLoadingIr ();
        json data = loading_ir;
        web_browser_component_.emitEventIfBrowserIsVisible ("on_loading_ir_updated_native",
                                                            {data.dump ()});
    };
}

juce::WebBrowserComponent::Options
LoadRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions
        .withNativeFunction (
            "load_ir_native",
            [&] (auto & var, auto complete)
            {
                IrSelection ir_selection;
                json::parse (var [0].toString ().toStdString ()).get_to (ir_selection);
                load_controller_.Load (ir_selection,
                                       [complete] (bool result) { complete ({result}); });
            })
        .withNativeFunction ("get_loading_ir_native",
                             [&] (auto & var, auto complete)
                             {
                                 const auto & loading_zone = load_controller_.GetLoadingIr ();
                                 json data = loading_zone;
                                 complete ({data.dump ()});
                             });
}
