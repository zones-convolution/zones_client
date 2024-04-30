#include "SidebarFooter.h"

#include "look_and_feel/LookAndFeel.h"

const PanelComponent::ColourPair SidebarFooter::kIrPanelGradient = {juce::Colours::darkmagenta,
                                                                    juce::Colours::darkorange};

SidebarFooter::SidebarFooter (const CurrentIrReader & ir_reader,
                              AudioGraphMetering & input_graph_metering,
                              AudioGraphMetering & output_graph_metering)
    : ir_reader_ (ir_reader)
    , ir_label_panel_ (ir_label_, kIrPanelGradient)
{
    addAndMakeVisible (ir_label_panel_);
    addAndMakeVisible (meter_component_);

    UpdateIrLabel ();
    lager::watch (ir_reader_, [&] (const auto &) { UpdateIrLabel (); });

    auto create_meter_delegate = [] (AudioGraphMetering & graph_metering, int channel_index)
    {
        return MeterComponent::ChannelMeterDelegate {
            .get_peak = [&, channel_index] ()
            { return graph_metering.GetChannelPeak (channel_index); },
            .is_clipping = [&, channel_index] ()
            { return graph_metering.GetChannelClipping (channel_index); },
            .reset_clipping = [&, channel_index] ()
            { graph_metering.ResetClipping (channel_index); }};
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
    layout.items.add (juce::FlexItem (meter_component_).withFlex (2.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void SidebarFooter::UpdateIrLabel ()
{
    static const auto kNoIrLabelText = "No Ir Loaded!";
    auto & current_ir_metadata = *ir_reader_;
    auto ir_label_text =
        current_ir_metadata.has_value () ? *current_ir_metadata->ir.title : kNoIrLabelText;
    ir_label_.setText (ir_label_text, juce::dontSendNotification);
}