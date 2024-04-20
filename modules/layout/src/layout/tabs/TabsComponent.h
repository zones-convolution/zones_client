#pragma once
#include "look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class TabsComponent : public juce::Component
{
public:
    void DisplayTab (juce::Component & tab);
    ~TabsComponent () override;

    void resized () override;

private:
    std::optional<std::reference_wrapper<juce::Component>> current_tab_;
};
