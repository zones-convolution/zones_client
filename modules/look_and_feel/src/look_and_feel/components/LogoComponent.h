#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class LogoComponent : public juce::Component
{
public:
    LogoComponent ();
    void paint (juce::Graphics & g) override;

private:
    std::unique_ptr<juce::Drawable> zones_logo_drawable_;
};