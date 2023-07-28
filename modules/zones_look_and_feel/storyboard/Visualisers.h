#pragma once
#include "spectrogram_visualiser/component/Graph3DComponent.h"
#include "spectrogram_visualiser/component/GraphComponent.h"
#include "spectrogram_visualiser/component/Osc2DComponent.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class Visualisers : public juce::Component
{
public:
    Visualisers ();
    void resized () override;

private:
    juce::Label visualisers_label_;
    Osc2DComponent osc2d_component_;
    GraphComponent graph_component_;
    Graph3DComponent graph3d_component_;
};
