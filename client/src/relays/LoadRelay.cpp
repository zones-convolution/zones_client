#include "LoadRelay.h"

#include "format/ZoneMetadataJson.h"

LoadRelay::LoadRelay (juce::WebBrowserComponent & web_browser_component,
                      LoadController & load_controller)
    : web_browser_component_ (web_browser_component)
    , load_controller_ (load_controller)
{
    load_controller_.OnLoadingIrUpdated = [&]
    {
        juce::MessageManager::callAsync (
            [&]
            {
                json data = load_controller_.GetLoadingIr ();
                JUCE_ASSERT_MESSAGE_THREAD;
                web_browser_component_.emitEventIfBrowserIsVisible ("on_loading_ir_updated_native",
                                                                    {data.dump ()});
            });
    };

    load_controller_.OnValidTargetFormatsUpdated = [&]
    {
        json data = load_controller_.GetValidTargetFormats ();
        JUCE_ASSERT_MESSAGE_THREAD;
        web_browser_component_.emitEventIfBrowserIsVisible (
            "on_valid_target_formats_updated_native", {data.dump ()});
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
                                       [complete, ir_selection] (bool result)
                                       {
                                           juce::MessageManager::callAsync (
                                               [result, complete]
                                               {
                                                   JUCE_ASSERT_MESSAGE_THREAD;
                                                   complete ({result});
                                               });
                                       });
            })
        .withNativeFunction ("get_loading_ir_native",
                             [&] (auto & var, auto complete)
                             {
                                 json data = load_controller_.GetLoadingIr ();
                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({data.dump ()});
                             })
        .withNativeFunction ("get_valid_target_formats_native",
                             [&] (auto & var, auto complete)
                             {
                                 json data = load_controller_.GetValidTargetFormats ();
                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({data.dump ()});
                             });
}
