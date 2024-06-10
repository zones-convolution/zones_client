#pragma once

#include "audio_engine/AudioGraphMetering.h"
#include "look_and_feel/components/PanelComponent.h"
#include "metering/MeterComponent.h"
#include "zone_repository/ZoneRepositoryModel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/reader.hpp>

class ZoneLabel : public juce::Component
{
public:
    ZoneLabel (const CurrentIrReader & ir_reader);
    void resized () override;

private:
    void UpdateLabels ();
    CurrentIrReader ir_reader_;
    juce::Label ir_label_;
    juce::Label zone_label_;
    juce::Label target_format_label_;
};

class SidebarFooter : public juce::Component
{
public:
    SidebarFooter (const CurrentIrReader & ir_reader,
                   AudioGraphMetering & input_graph_metering,
                   AudioGraphMetering & output_graph_metering);

    void resized () override;

private:
    void ConfigureMeter ();

    static const PanelComponent::ColourPair kZonePanelGradient;
    ZoneLabel zone_label_;
    PanelComponent zone_label_panel_;

    AudioGraphMetering & input_graph_metering_;
    AudioGraphMetering & output_graph_metering_;
    MeterComponent meter_component_;
};
