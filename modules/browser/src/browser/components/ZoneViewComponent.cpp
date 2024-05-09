#include "ZoneViewComponent.h"

#include "look_and_feel/LookAndFeel.h"

ZoneViewComponent::ZoneViewComponent (lager::context<Action> & context)
    : context_ (context)
{
    addAndMakeVisible (label_);

    addAndMakeVisible (ir_selection_);

    load_.onClick = [&]
    {
        auto & irs = zone_metadata_.irs;
        auto selected_id = ir_selection_.getSelectedItemIndex ();
        auto selected_ir = irs [selected_id];

        context.dispatch (LoadIrAction {.ir_selection = {.zone = zone_metadata_,
                                                         .ir = selected_ir,
                                                         .target_format = TargetFormat::kStereo}});
    };

    addAndMakeVisible (load_);
}

void ZoneViewComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (LookAndFeel::LabelFlexItem (label_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::ComboFlexItem (ir_selection_));
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
