#include "BannerGrid.h"

#include "look_and_feel/LookAndFeel.h"

BannerGrid::BannerGrid (std::vector<std::unique_ptr<juce::Component>> & grid_components)
    : grid_components_ (grid_components)
{
    addAndMakeVisible (viewport_);
    viewport_.setViewedComponent (&content_, false);
}

void BannerGrid::Update ()
{
    content_.removeAllChildren ();
    for (auto & child : grid_components_)
        content_.addAndMakeVisible (*child);
    resized ();
}

float BannerGrid::GetRequiredContentWidth ()
{
    auto num_children = grid_components_.size ();
    if (num_children == 0)
        return 0.0f;
    return (num_children * getHeight ()) + ((num_children - 1) * LookAndFeel::kDoubleGap);
}

void BannerGrid::PerformLayout ()
{
    auto viewport_visible_height = viewport_.getMaximumVisibleHeight ();
    auto content_bounds = getLocalBounds ()
                              .withWidth (GetRequiredContentWidth ())
                              .withHeight (viewport_visible_height);
    if (viewport_.canScrollHorizontally ())
        content_bounds.removeFromBottom (LookAndFeel::kPadding);
    content_.setBounds (content_bounds);

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.setGap (juce::Grid::Px {LookAndFeel::kDoubleGap});

    grid.templateRows = {Track (Fr (1))};

    for (auto & child : grid_components_)
    {
        grid.templateColumns.add (Track (Fr (1)));
        grid.items.add (juce::GridItem (*child));
    }

    grid.performLayout (content_bounds);
}

void BannerGrid::resized ()
{
    viewport_.setBounds (getLocalBounds ());

    auto viewport_visible_height = viewport_.getMaximumVisibleHeight ();
    PerformLayout ();
    if (viewport_.getViewHeight () != viewport_visible_height)
        PerformLayout ();
}