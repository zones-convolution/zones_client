#pragma once
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

class DraggableOrientation
{
public:
    void SetBounds (juce::Rectangle<int> bounds);
    void MouseDown (juce::Point<float> mouse_pos);
    void MouseDrag (juce::Point<float> mouse_pos);

    std::atomic<float> x_rotation;
    std::atomic<float> y_rotation;

private:
    static constexpr float kDragSpeed = 0.2f;
    juce::Rectangle<int> bounds_;
    juce::Point<float> last_mouse_;

    juce::Point<float> MousePosToProportion (juce::Point<float> mouse_pos) const;
};
