#pragma once
#include "visualiser/component/WaterfallComponent.h"
#include "zones_look_and_feel/BoxIcons.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/DividerComponent.h"
#include "zones_look_and_feel/components/IconTextButton.h"

#include <juce_gui_basics/juce_gui_basics.h>

class Visualisers : public juce::Component
{
public:
    Visualisers ();
    void resized () override;

private:
    juce::Label visualisers_label_;
    IconTextButton load_waterfall_ {"Load Waterfall", BoxIcons::kBxUpload};
    DividerComponent divider_;

    WaterfallComponent waterfall_component_;
    std::unique_ptr<juce::FileChooser> waterfall_chooser_;
};
