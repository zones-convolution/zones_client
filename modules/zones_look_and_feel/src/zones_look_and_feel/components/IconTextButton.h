#pragma once
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/IconComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

class IconTextButton : public juce::Button
{
public:
    IconTextButton ();
    explicit IconTextButton (const juce::String & name);
    IconTextButton (const juce::String & name, const juce::String & icon);
    IconTextButton (const juce::String & name,
                    const juce::String & icon,
                    const juce::String & tool_tip);

    ~IconTextButton () override;

    void paintButton (juce::Graphics &, bool, bool) override;
    void resized () override;

    void SetIcon (const juce::String & icon);

private:
    juce::Label label_;
    IconComponent icon_;
};
