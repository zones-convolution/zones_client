#include "VisualiserRelay.h"

#include "immer/vector.hpp"

VisualiserRelay::VisualiserRelay (juce::WebBrowserComponent & web_browser_component,
                                  VisualiserController & visualiser_controller)
    : web_browser_component_ (web_browser_component)
    , visualiser_controller_ (visualiser_controller)
{
    connections_ += {
        visualiser_controller.OnVisualiserRendered.connect (
            [&] ()
            {
                JUCE_ASSERT_MESSAGE_THREAD;
                web_browser_component_.emitEventIfBrowserIsVisible ("on_visualiser_render_native",
                                                                    {});
            }),
    };
}

juce::WebBrowserComponent::Resource VisualiserRelay::GetVisualiserResource () const
{
    juce::WebBrowserComponent::Resource resource;
    resource.mimeType = "application/octet-stream";

    auto spectrogram = visualiser_controller_.GetVisualiserRender ();
    if (spectrogram)
    {
        auto spec_data = spectrogram.value ();
        auto size = spec_data->getNumChannels () * spec_data->getNumSamples ();
        resource.data.resize (size);
        std::memcpy (resource.data.data (), spec_data->getReadPointer (0), size);
    }
    else
    {
        resource.data = {};
    }

    return resource;
}
