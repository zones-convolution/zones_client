#include "EngineRelay.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

static void to_json (json & data, const EngineLoading & engine_loading)
{
    data = json {
        {"irEngineLoading", engine_loading.ir_engine_loading},
        {"convolutionEngineLoading", engine_loading.convolution_engine_loading},
    };
}

EngineRelay::EngineRelay (juce::WebBrowserComponent & web_browser_component,
                          IrEngine & ir_engine,
                          zones::ConvolutionEngine & convolution_engine)
    : web_browser_component_ (web_browser_component)
    , ir_engine_ (ir_engine)
    , convolution_engine_ (convolution_engine)
{
    connections_ += {ir_engine_.OnLoadingUpdated.connect ([&] { OnEngineUpdatedEmitEvent (); })};
    convolution_engine_.add (this);
}

EngineRelay::~EngineRelay ()
{
    convolution_engine_.remove (this);
}

void EngineRelay::OnLoadingUpdated ()
{
    OnEngineUpdatedEmitEvent ();
}

juce::WebBrowserComponent::Options
EngineRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions.withNativeFunction (
        "get_engine_loading_native",
        [&] (auto & var, auto complete)
        {
            json data =
                EngineLoading {.convolution_engine_loading = convolution_engine_.IsLoading (),
                               .ir_engine_loading = ir_engine_.IsLoading ()};
            JUCE_ASSERT_MESSAGE_THREAD;
            complete ({data.dump ()});
        });
}

void EngineRelay::OnEngineUpdatedEmitEvent ()
{
    json data = EngineLoading {.convolution_engine_loading = convolution_engine_.IsLoading (),
                               .ir_engine_loading = ir_engine_.IsLoading ()};
    JUCE_ASSERT_MESSAGE_THREAD;
    web_browser_component_.emitEventIfBrowserIsVisible ("on_engine_loading_updated",
                                                        {data.dump ()});
}