#pragma once

#include "look_and_feel/components/PanelComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

class BannerGrid : public juce::Component
{
public:
    explicit BannerGrid (std::vector<std::unique_ptr<juce::Component>> & grid_components);

    void Update ();
    void resized () override;

private:
    float GetRequiredContentWidth ();
    void PerformLayout ();
    std::vector<std::unique_ptr<juce::Component>> & grid_components_;

    juce::Component content_;
    juce::Viewport viewport_;
};