#include "LoadRelay.h"

#include "format/ZoneMetadataJson.h"

LoadRelay::LoadRelay (juce::WebBrowserComponent & web_browser_component,
                      LoadController & load_controller)
    : web_browser_component_ (web_browser_component)
    , load_controller_ (load_controller)
{
    connections_ += {load_controller_.OnLoadingIrUpdated.connect (
                         [&]
                         {
                             json data = load_controller_.GetLoadingIr ();
                             JUCE_ASSERT_MESSAGE_THREAD;
                             web_browser_component_.emitEventIfBrowserIsVisible (
                                 "on_loading_ir_updated_native", {data.dump ()});
                         }),
                     load_controller_.OnCurrentIrUpdated.connect (
                         [&]
                         {
                             json data = load_controller_.GetCurrentIr ();
                             JUCE_ASSERT_MESSAGE_THREAD;
                             web_browser_component_.emitEventIfBrowserIsVisible (
                                 "on_current_ir_updated_native", {data.dump ()});
                         }),
                     load_controller_.OnValidTargetFormatsUpdated.connect (
                         [&]
                         {
                             json data = load_controller_.GetValidTargetFormats ();
                             JUCE_ASSERT_MESSAGE_THREAD;
                             web_browser_component_.emitEventIfBrowserIsVisible (
                                 "on_valid_target_formats_updated_native", {data.dump ()});
                         })};
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
                juce::WeakReference<LoadRelay> weak_self = this;

                load_controller_.Load (ir_selection,
                                       [complete, ir_selection, weak_self] (bool result)
                                       {
                                           JUCE_ASSERT_MESSAGE_THREAD;
                                           if (weak_self.get ())
                                               complete ({result});
                                       });
            })
        .withNativeFunction ("get_loading_ir_native",
                             [&] (auto & var, auto complete)
                             {
                                 json data = load_controller_.GetLoadingIr ();
                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({data.dump ()});
                             })
        .withNativeFunction ("get_current_ir_native",
                             [&] (auto & var, auto complete)
                             {
                                 json data = load_controller_.GetCurrentIr ();
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
