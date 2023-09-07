#pragma once

#include "audio_engine/AudioGraphMetering.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"
#include "metering/MeterComponent.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/PanelComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/reader.hpp>

class SidebarFooter : public juce::Component
{
public:
    SidebarFooter (const lager::reader<CurrentProjectIrOptional> & project_ir_reader,
                   AudioGraphMetering & input_graph_metering,
                   AudioGraphMetering & output_graph_metering);

    void resized () override;

private:
    lager::reader<CurrentProjectIrOptional> project_ir_reader_;

    static const PanelComponent::ColourPair kIrPanelGradient;
    juce::Label ir_label_;
    PanelComponent ir_label_panel_;

    MeterComponent meter_component_;

    void UpdateIrLabel ();
};
