#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class WebZonesRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    WebZonesRelay (juce::WebBrowserComponent & web_browser_component);
    ~WebZonesRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    juce::WebBrowserComponent & web_browser_component_;
    JUCE_DECLARE_WEAK_REFERENCEABLE (WebZonesRelay)
};
