#pragma once

#include "../BrowserModel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class ZoneViewComponent : public juce::Component
{
public:
    ZoneViewComponent ();
    void Update (const ZoneView & zone_view);
    void resized () override;

private:
    juce::Label label_ {"Zone View", "Zone View"};
};
