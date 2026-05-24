#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class BrowserRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    BrowserRelay () = default;
    ~BrowserRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;
};
