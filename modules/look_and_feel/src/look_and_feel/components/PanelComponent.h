#pragma once

#include "../LookAndFeel.h"
#include "juce_gui_basics/juce_gui_basics.h"

class PanelComponent : public juce::Component
{
public:
    using ColourPair = std::pair<juce::Colour, juce::Colour>;

    explicit PanelComponent (juce::Component & child, bool apply_content_rounding = false);
    explicit PanelComponent (juce::Component & child,
                             ColourPair horizontal_gradient_colours,
                             bool apply_content_rounding = false);
    ~PanelComponent () override;

    void paint (juce::Graphics & g) override;

    void resized () override;

    void paintOverChildren (juce::Graphics & g) override;

private:
    bool apply_content_rounding_;
    juce::Component & child_;

    std::optional<ColourPair> background_gradient_ = std::nullopt;
};