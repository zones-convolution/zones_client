#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "visualiser/component/WaterfallComponent.h"
#include "zones_look_and_feel/components/PanelComponent.h"

class VisualiserPanel : public juce::Component
{
public:
    VisualiserPanel ();
    void resized () override;

    WaterfallComponent waterfall_component_;

private:
    PanelComponent waterfall_panel_{waterfall_component_};
};


