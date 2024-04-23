#include "Top10ViewComponent.h"

#include "look_and_feel/LookAndFeel.h"

ZoneCardComponent::ZoneCardComponent (const std::string & image_path, const std::string & name)
{
    name_label_.setText (name, juce::dontSendNotification);
    addAndMakeVisible (name_label_);
    background_image_ = juce::ImageCache::getFromFile (juce::File {image_path});
}

void ZoneCardComponent::paint (juce::Graphics & g)
{
    auto border = juce::Path ();
    border.addRoundedRectangle (getLocalBounds ().toFloat (), LookAndFeel::kRounding);
    g.reduceClipRegion (border);
    g.drawImage (
        background_image_, getLocalBounds ().toFloat (), juce::RectanglePlacement::fillDestination);
}

void ZoneCardComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.items.add (LookAndFeel::LabelFlexItem (name_label_));
    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kComponentInset));
}

Top10ContentComponent::Top10ContentComponent ()
{
    for (auto & zone_card : zone_cards_)
        addAndMakeVisible (zone_card);
}

void Top10ContentComponent::resized ()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.setGap (juce::Grid::Px {LookAndFeel::kDoubleGap});

    grid.templateRows = {Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1))};
    grid.templateColumns = {Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1))};

    for (auto & zone_card : zone_cards_)
        grid.items.add (juce::GridItem (zone_card));

    grid.performLayout (getLocalBounds ());
}

float Top10ContentComponent::GetRequiredHeightForLayout ()
{
    return (zone_cards_.size () / 4.0f) * 280.0f;
}

Top10ViewComponent::Top10ViewComponent ()
{
    addAndMakeVisible (viewport_);
    viewport_.setViewedComponent (&top_10_content_component_, false);
}

void Top10ViewComponent::PerformLayout ()
{
    auto viewport_visible_width = viewport_.getMaximumVisibleWidth ();
    auto content_bounds = getLocalBounds ()
                              .withWidth (viewport_.getMaximumVisibleWidth ())
                              .withHeight (top_10_content_component_.GetRequiredHeightForLayout ());
    if (viewport_.canScrollVertically ())
        content_bounds.removeFromRight (LookAndFeel::kPadding);
    top_10_content_component_.setBounds (content_bounds);
}

void Top10ViewComponent::resized ()
{
    viewport_.setBounds (getLocalBounds ());

    auto viewport_visible_width = viewport_.getMaximumVisibleWidth ();
    PerformLayout ();
    if (viewport_.getViewWidth () != viewport_visible_width)
        PerformLayout ();
}

void Top10ViewComponent::Update (const Top10View & top_10_view)
{
}
