#pragma once
#include "LabelContainer.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class DiscreteLevelLabels : public juce::Component
{
public:
    DiscreteLevelLabels (LabelContainer & label_height_container);

    void resized () override;

private:
    LabelContainer & label_height_container_;
    std::vector<std::unique_ptr<juce::Label>> labels_;
};
