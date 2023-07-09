#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class StoryboardComponent : public juce::Component
{
public:
    StoryboardComponent ();

    void paint (juce::Graphics &) override;
    void resized () override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StoryboardComponent)
};