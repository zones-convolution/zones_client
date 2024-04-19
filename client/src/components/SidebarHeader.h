#pragma once
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/LookAndFeel.h"
#include "look_and_feel/components/IconComponent.h"
#include "look_and_feel/components/LogoComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

class SidebarHeader : public juce::Component
{
public:
    SidebarHeader ();
    void resized () override;

private:
    LogoComponent logo_component_;
    IconComponent icon_sad_;
};
