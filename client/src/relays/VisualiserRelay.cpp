#include "VisualiserRelay.h"

VisualiserRelay::VisualiserRelay (juce::WebBrowserComponent & web_browser_component,
                                  VisualiserController & visualiser_controller)

{
}

VisualiserRelay::~VisualiserRelay ()
{
}

juce::WebBrowserComponent::Options
VisualiserRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions;
}
