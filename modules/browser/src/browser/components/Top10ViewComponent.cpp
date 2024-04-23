#include "Top10ViewComponent.h"

Top10ViewComponent::Top10ViewComponent ()
{
    addAndMakeVisible (label_);
}

void Top10ViewComponent::resized ()
{
    label_.setBounds (getLocalBounds ());
}

void Top10ViewComponent::Update (const Top10View & top_10_view)
{
}
