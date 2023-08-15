#include "SidebarFooter.h"

const PanelComponent::ColourPair SidebarFooter::kIrPanelGradient = {juce::Colours::darkmagenta,
                                                                    juce::Colours::darkorange};

SidebarFooter::SidebarFooter (const lager::reader<CurrentProjectIrOptional> & project_ir_reader,
                              AudioGraphMetering & audio_graph_metering)
    : project_ir_reader_ (project_ir_reader)
    , ir_label_panel_ (ir_label_, kIrPanelGradient)
    , meter_component_ (audio_graph_metering)
{
    addAndMakeVisible (ir_label_panel_);
    addAndMakeVisible (meter_component_);

    UpdateIrLabel ();
    lager::watch (project_ir_reader_, [&] (const auto &) { UpdateIrLabel (); });
}

void SidebarFooter::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    layout.items.add (juce::FlexItem (ir_label_panel_).withFlex (2.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (meter_component_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void SidebarFooter::UpdateIrLabel ()
{
    static const auto kNoIrLabelText = "No Ir Loaded!";
    auto ir_label_text =
        project_ir_reader_->has_value () ? project_ir_reader_->value () : kNoIrLabelText;
    ir_label_.setText (ir_label_text, juce::dontSendNotification);
}