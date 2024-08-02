#include "VisualiserRelay.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using BoxedBuffer = immer::box<juce::AudioBuffer<float>>;

static void BoxedBufferToJson (json & json, BoxedBuffer render)
{
    json = json::array ();

    auto num_channels = render->getNumChannels ();
    auto num_samples = render->getNumSamples ();

    for (auto channel_index = 0; channel_index < num_channels; ++channel_index)
    {
        auto channel_json = json::array ();
        for (auto sample_index = 0; sample_index < num_samples; ++sample_index)
            channel_json.push_back (render->getSample (channel_index, sample_index));
        json.push_back (channel_json);
    }
}

VisualiserRelay::VisualiserRelay (juce::WebBrowserComponent & web_browser_component,
                                  VisualiserController & visualiser_controller)
    : web_browser_component_ (web_browser_component)
    , visualiser_controller_ (visualiser_controller)
{
    connections_ += {
        visualiser_controller.OnVisualiserRendered.connect (
            [&] (BoxedBuffer render_result)
            {
                JUCE_ASSERT_MESSAGE_THREAD;
                json data;
                BoxedBufferToJson (data, render_result);
                web_browser_component_.emitEventIfBrowserIsVisible ("on_visualiser_render_native",
                                                                    {data.dump ()});
            }),
    };
}

juce::WebBrowserComponent::Options
VisualiserRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions.withNativeFunction ("get_visualiser_render_native",
                                              [&] (auto & var, auto complete)
                                              {
                                                  JUCE_ASSERT_MESSAGE_THREAD;
                                                  auto render =
                                                      visualiser_controller_.GetVisualiserRender ();

                                                  if (render)
                                                  {
                                                      json data;
                                                      BoxedBufferToJson (data, *render);
                                                      complete ({data.dump ()});
                                                  }
                                              });
}
