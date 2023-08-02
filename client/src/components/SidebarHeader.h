#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <zones_look_and_feel/components/LogoComponent.h>
class SidebarHeader : public juce::Component
{
public:
    SidebarHeader ();
    void resized () override;

private:
    LogoComponent logo_component_;
};
