#include "UserZoneCard.h"

#include "look_and_feel/LookAndFeel.h"

UserZoneCard::UserZoneCard (const IrMetadata & ir_metadata,
                            const lager::reader<std::optional<std::filesystem::path>> & reader)
    : ir_metadata_ (ir_metadata)
    , ir_reader_ (reader)
{
    addAndMakeVisible (panel_);

    ir_title_.setText (*ir_metadata.name + " | " +
                           IrMetadata::ChannelFormatToString (*ir_metadata.channel_format),
                       juce::dontSendNotification);
    addAndMakeVisible (ir_title_);

    load_.onClick = [&]
    {
        if (OnLoad)
            OnLoad ();
    };
    addAndMakeVisible (load_);

    view_.onClick = [&]
    {
        if (OnView)
            OnView ();
    };
    addAndMakeVisible (view_);

    lager::watch (ir_reader_,
                  [&, ir_metadata] (const auto & path)
                  {
                      if (path.has_value ())
                          load_.setEnabled (path->stem () != *ir_metadata.name);
                      else
                          load_.setEnabled (true);
                  });
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
