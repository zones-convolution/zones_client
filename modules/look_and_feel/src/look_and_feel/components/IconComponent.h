#pragma once
#include "../LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <optional>

class IconComponent : public juce::Component
{
public:
    void paint (juce::Graphics & g) override;
    void SetIcon (const juce::String & icon);
    void SetColour (const juce::Colour & colour);

private:
    juce::String icon_;
    std::optional<juce::Colour> colour_;
};
