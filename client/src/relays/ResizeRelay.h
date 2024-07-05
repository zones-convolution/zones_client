#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class ResizeRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    ResizeRelay () = default;
    ~ResizeRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

    void Setup (juce::Component * component, juce::ComponentBoundsConstrainer * constrainer);

private:
    void HandleDragBegin (juce::Point<int> start_pos);
    void HandleDragEnd ();
    void HandleDrag (juce::Point<int> drag);

    juce::Point<int> start_pos_;
    juce::Rectangle<int> original_bounds_;
    juce::Component * component_ = nullptr;
    juce::ComponentBoundsConstrainer * constrainer_ = nullptr;
};
