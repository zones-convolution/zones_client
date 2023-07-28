#include "DraggableOrientation.h"

void DraggableOrientation::SetBounds (juce::Rectangle<int> bounds)
{
    bounds_ = bounds;
}

void DraggableOrientation::MouseDown (juce::Point<float> mouse_pos)
{
    last_mouse_ = MousePosToProportion (mouse_pos.toFloat ());
}

void DraggableOrientation::MouseDrag (juce::Point<float> mouse_pos)
{
    auto proportion = MousePosToProportion (mouse_pos.toFloat ());
    auto delta_x = proportion.getX () - last_mouse_.getX ();
    auto delta_y = proportion.getY () - last_mouse_.getY ();

    y_rotation = y_rotation.load () + (delta_y * kDragSpeed);
    x_rotation = x_rotation.load () + (delta_x * kDragSpeed);

    last_mouse_ = proportion;
}

juce::Point<float> DraggableOrientation::MousePosToProportion (juce::Point<float> mouse_pos) const
{
    auto scale = juce::jmin (bounds_.getWidth (), bounds_.getHeight ()) / 2;
    jassert (scale > 0);
    return {(mouse_pos.x - (float) bounds_.getCentreX ()) / (float) scale,
            ((float) bounds_.getCentreY () - mouse_pos.y) / (float) scale};
}
