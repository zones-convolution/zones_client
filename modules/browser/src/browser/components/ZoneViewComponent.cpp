#include "ZoneViewComponent.h"

#include "look_and_feel/LookAndFeel.h"

ZoneViewComponent::ZoneViewComponent (lager::context<Action> & context,
                                      const lager::reader<Model> & model)
    : context_ (context)
    , valid_target_formats_reader_ (
          model [&Model::zone_repository_model][&ZoneRepositoryModel::valid_target_formats])
{
    addAndMakeVisible (label_);

    ir_selection_.onChange = [&] { UpdateTargetFormats (); };

    addAndMakeVisible (ir_selection_);
    addAndMakeVisible (target_format_selection_);

    load_.onClick = [&]
    {
        auto & irs = zone_metadata_.irs;
        auto selected_id = ir_selection_.getSelectedItemIndex ();
        auto selected_ir = irs [selected_id];
        auto selected_target_format = GetTargetFormatForString (
            target_format_selection_.getItemText (target_format_selection_.getSelectedItemIndex ())
                .toStdString ());
        if (selected_target_format.has_value ())
            context.dispatch (
                LoadIrAction {.ir_selection = {.zone = zone_metadata_,
                                               .ir = selected_ir,
                                               .target_format = selected_target_format.value ()}});
    };

    addAndMakeVisible (load_);
}
void ZoneViewComponent::UpdateTargetFormats ()
{
    target_format_selection_.clear (juce::dontSendNotification);

    auto valid_target_formats = valid_target_formats_reader_.get ();
    auto & irs = zone_metadata_.irs;
    auto selected_id = ir_selection_.getSelectedItemIndex ();
    auto selected_ir = irs [selected_id];

    if ((selected_ir.channel_format.has_value () && selected_ir.position_map.has_value ()))
    {
        IrFormatData ir_format_data {.channel_format = selected_ir.channel_format.value (),
                                     .position_map = selected_ir.position_map.value ()};

        for (auto i = 0; i < valid_target_formats.size (); ++i)
        {
            auto target_format = valid_target_formats [i];
            if (ir_format_data.SupportsTarget (target_format))
            {
                target_format_selection_.addItem (GetStringForTargetFormat (target_format), i + 1);
            }
        }
    }
    load_.setEnabled (! target_format_selection_.getNumItems () == 0);

    target_format_selection_.setSelectedItemIndex (0, juce::dontSendNotification);
}

void ZoneViewComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::LabelFlexItem (label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ComboFlexItem (ir_selection_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ComboFlexItem (target_format_selection_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ButtonFlexItem (load_));

    layout.performLayout (getLocalBounds ());
}

void ZoneViewComponent::Update (const ZoneView & zone_view)
{
    zone_metadata_ = zone_view.zone_metadata;
    auto & zone_metadata = zone_view.zone_metadata;
    label_.setText (zone_metadata.title, juce::dontSendNotification);

    ir_selection_.clear (juce::dontSendNotification);
    auto & irs = zone_metadata.irs;
    for (auto i = 0; i < irs.size (); ++i)
        ir_selection_.addItem (*irs [i].title, i + 1);

    ir_selection_.setSelectedItemIndex (0);
}
