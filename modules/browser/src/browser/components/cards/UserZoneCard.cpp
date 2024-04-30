#include "UserZoneCard.h"

#include "look_and_feel/LookAndFeel.h"

UserZoneCard::UserZoneCard (const ZoneMetadata & zone_metadata,
                            const lager::reader<Model> & model,
                            lager::context<Action> & context,
                            lager::context<BrowserAction> & browser_context)
    : zone_metadata_ (zone_metadata)
    , context_ (context)
    , ir_reader_ (model [&Model::zone_repository_model][&ZoneRepositoryModel::current_ir])
    , browser_context_ (browser_context)
{
    addAndMakeVisible (panel_);

    zone_title_.setText (zone_metadata.title, juce::dontSendNotification);
    addAndMakeVisible (zone_title_);

    load_.onClick = [&]
    {
        context_.dispatch (LoadIrAction {.ir_selection {.zone = zone_metadata_,
                                                        .ir = zone_metadata_.irs [0],
                                                        .target_format = TargetFormat::kStereo}});
    };
    addAndMakeVisible (load_);

    view_.onClick = [&]
    { browser_context_.dispatch (LoadZoneAction {.zone_metadata = zone_metadata_}); };
    addAndMakeVisible (view_);

    //    lager::watch (ir_reader_,
    //                  [&] (const auto & current_ir_metadata)
    //                  { load_.setEnabled (current_ir_metadata.zone != zone_metadata_); });
    //    load_.setEnabled (*ir_reader_ != ir_metadata_);
}

void UserZoneCard::resized ()
{
    panel_.setBounds (getLocalBounds ());

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;
    layout.alignItems = juce::FlexBox::AlignItems::flexEnd;

    layout.items.add (LookAndFeel::LabelFlexItem (zone_title_));
    layout.items.add (juce::FlexItem ().withFlex (1.0f));
    layout.items.add (LookAndFeel::MediumIconFlexItem (load_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::MediumIconFlexItem (view_));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}
