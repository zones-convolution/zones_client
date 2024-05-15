#include "SidebarFooter.h"

#include "look_and_feel/LookAndFeel.h"

const PanelComponent::ColourPair SidebarFooter::kZonePanelGradient = {juce::Colours::darkmagenta,
                                                                      juce::Colours::darkorange};

ZoneLabel::ZoneLabel (const CurrentIrReader & ir_reader)
    : ir_reader_ (ir_reader)
{
    addAndMakeVisible (ir_label_);
    addAndMakeVisible (zone_label_);
    addAndMakeVisible (target_format_label_);

    UpdateLabels ();
    lager::watch (ir_reader_, [&] (const auto &) { UpdateLabels (); });
}

void ZoneLabel::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.items.add (juce::FlexItem {}.withFlex (7.f));
    layout.items.add (juce::FlexItem (ir_label_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (zone_label_).withFlex (1.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (target_format_label_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void ZoneLabel::UpdateLabels ()
{
    static const auto kNoIrLabelText = "No Ir Loaded!";
    static const auto kNoZoneLabelText = "";
    static const auto kNoTargetFormatLabelText = "";

    auto & current_ir_metadata = *ir_reader_;

    auto ir_label_text =
        current_ir_metadata.has_value () ? *current_ir_metadata->ir.title : kNoIrLabelText;
    ir_label_.setText (ir_label_text, juce::dontSendNotification);

    auto zone_label_text =
        current_ir_metadata.has_value () ? current_ir_metadata->zone.title : kNoZoneLabelText;
    zone_label_.setText (zone_label_text, juce::dontSendNotification);

    auto target_format_label_text =
        current_ir_metadata.has_value ()
            ? GetStringForTargetFormat (current_ir_metadata->target_format)
            : kNoTargetFormatLabelText;
    target_format_label_.setText (target_format_label_text, juce::dontSendNotification);
}

SidebarFooter::SidebarFooter (const CurrentIrReader & ir_reader,
                              AudioGraphMetering & input_graph_metering,
                              AudioGraphMetering & output_graph_metering)
    : zone_label_ (ir_reader)
    , zone_label_panel_ (zone_label_, kZonePanelGradient)
{
    addAndMakeVisible (zone_label_panel_);
    addAndMakeVisible (meter_component_);

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

    layout.items.add (juce::FlexItem {zone_label_panel_}.withFlex (2.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (meter_component_).withFlex (2.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}
