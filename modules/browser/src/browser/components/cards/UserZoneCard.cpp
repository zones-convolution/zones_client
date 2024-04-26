#include "UserZoneCard.h"

#include "look_and_feel/LookAndFeel.h"

UserZoneCard::UserZoneCard (const IrMetadata & ir_metadata,
                            const lager::reader<Model> & model,
                            lager::context<Action> & context,
                            lager::context<BrowserAction> & browser_context)
    : ir_metadata_ (ir_metadata)
    , context_ (context)
    , ir_reader_ (model [&Model::ir_repository_model][&IrRepositoryModel::current_ir_metadata])
    , browser_context_ (browser_context)
{
    addAndMakeVisible (panel_);

    ir_title_.setText (*ir_metadata.name + " | " +
                           IrMetadata::ChannelFormatToString (*ir_metadata.channel_format),
                       juce::dontSendNotification);
    addAndMakeVisible (ir_title_);

    load_.onClick = [&]
    {
        context_.dispatch (
            LoadIrAction {.ir_metadata = ir_metadata_, .target_format = TargetFormat::kStereo});
    };
    addAndMakeVisible (load_);

    view_.onClick = [&]
    { browser_context_.dispatch (LoadZoneAction {.ir_metadata = ir_metadata_}); };
    addAndMakeVisible (view_);

    lager::watch (ir_reader_,
                  [&] (const auto & current_ir_metadata)
                  { load_.setEnabled (current_ir_metadata != ir_metadata_); });
    load_.setEnabled (*ir_reader_ != ir_metadata_);
}

void UserZoneCard::resized ()
{
    panel_.setBounds (getLocalBounds ());

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;
    layout.alignItems = juce::FlexBox::AlignItems::flexEnd;

    layout.items.add (LookAndFeel::LabelFlexItem (ir_title_));
    layout.items.add (juce::FlexItem ().withFlex (1.0f));
    layout.items.add (LookAndFeel::MediumIconFlexItem (load_));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (LookAndFeel::MediumIconFlexItem (view_));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}
