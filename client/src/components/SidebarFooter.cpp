#include "SidebarFooter.h"

const PanelComponent::ColourPair SidebarFooter::kIrPanelGradient = {juce::Colours::darkmagenta,
                                                                    juce::Colours::darkorange};

SidebarFooter::SidebarFooter (const lager::reader<CurrentProjectIrOptional> & project_ir_reader,
                              AudioGraphMetering & input_graph_metering,
                              AudioGraphMetering & output_graph_metering)
    : project_ir_reader_ (project_ir_reader)
    , ir_label_panel_ (ir_label_, kIrPanelGradient)
{
    addAndMakeVisible (ir_label_panel_);
    addAndMakeVisible (meter_component_);

    UpdateIrLabel ();
    lager::watch (project_ir_reader_, [&] (const auto &) { UpdateIrLabel (); });

    auto create_meter_delegate = [] (AudioGraphMetering & graph_metering, int channel_index)
    {
        return MeterComponent::ChannelMeterDelegate {
            .get_peak = [&, channel_index] ()
            { return graph_metering.GetChannelPeak (channel_index); },
            .is_clipping = [&, channel_index] ()
            { return graph_metering.GetChannelClipping (channel_index); }};
    };

    meter_component_.SetConfiguration ({{
                                            create_meter_delegate (input_graph_metering, 0),
                                            create_meter_delegate (input_graph_metering, 1),
                                        },
                                        {
                                            create_meter_delegate (output_graph_metering, 0),
                                            create_meter_delegate (output_graph_metering, 1),
                                        }});
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