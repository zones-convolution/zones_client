#pragma once

#include "Buttons.h"
#include "Combos.h"
#include "zones_look_and_feel/components/PanelComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

class StoryboardComponent : public juce::Component
{
public:
    StoryboardComponent ();

    void paint (juce::Graphics &) override;
    void resized () override;

private:
    Buttons buttons_;
    PanelComponent button_panel_ {buttons_};

    Combos combos_;
    PanelComponent combos_panel_ {combos_};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StoryboardComponent)
};