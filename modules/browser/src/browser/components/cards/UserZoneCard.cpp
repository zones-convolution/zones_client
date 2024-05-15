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
    , valid_target_formats_reader_ (
          model [&Model::zone_repository_model][&ZoneRepositoryModel::valid_target_formats])
{
    addAndMakeVisible (panel_);

    zone_title_.setText (zone_metadata.title, juce::dontSendNotification);
    addAndMakeVisible (zone_title_);

    load_.setEnabled (GetValidIrSelection ().has_value ());
    load_.onClick = [&] { context_.dispatch (LoadIrAction {GetValidIrSelection ().value ()}); };
    addAndMakeVisible (load_);

    view_.onClick = [&]
    { browser_context_.dispatch (LoadZoneAction {.zone_metadata = zone_metadata_}); };
    addAndMakeVisible (view_);

    //    lager::watch (ir_reader_,
    //                  [&] (const auto & current_ir_metadata)
    //                  { load_.setEnabled (current_ir_metadata.zone != zone_metadata_); });
    //    load_.setEnabled (*ir_reader_ != ir_metadata_);
}

std::optional<IrSelection> UserZoneCard::GetValidIrSelection ()
{
    auto valid_target_formats = valid_target_formats_reader_.get ();
    for (auto & ir : zone_metadata_.irs)
    {
        if ((ir.channel_format.has_value () && ir.position_map.has_value ()))
        {
            IrFormatData ir_format_data {.channel_format = ir.channel_format.value (),
                                         .position_map = ir.position_map.value ()};

            for (auto i = 0; i < valid_target_formats.size (); ++i)
            {
                auto target_format = valid_target_formats [i];
                if (ir_format_data.SupportsTarget (target_format))
                {
                    return IrSelection {
                        .zone = zone_metadata_, .ir = ir, .target_format = target_format};
                }
            }
        }
    }
    return std::nullopt;
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
