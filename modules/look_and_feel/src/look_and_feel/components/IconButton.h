#pragma once

#include "IconComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

class IconButton : public juce::Button
{
public:
    IconButton ();
    explicit IconButton (const juce::String & name);
    IconButton (const juce::String & name, const juce::String & icon);
    IconButton (const juce::String & name,
                const juce::String & icon,
                const juce::String & tool_tip);

    ~IconButton () override = default;

    void paintButton (juce::Graphics &, bool, bool) override;

    void SetIcon (const juce::String & icon);

private:
    juce::String icon_;
};