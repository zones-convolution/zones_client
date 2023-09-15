#include "EditorComponent.h"

EditorComponent::EditorComponent (juce::AudioProcessorValueTreeState & parameter_tree)
    : io_ (parameter_tree)
    , ir_engine_ (parameter_tree)
{
    addAndMakeVisible (io_panel_);
    addAndMakeVisible (visualiser_panel_);
    addAndMakeVisible (ir_engine_panel_);
}

void EditorComponent::resized ()
{
    juce::FlexBox right_layout;
    right_layout.flexDirection = juce::FlexBox::Direction::column;

    right_layout.items.add (juce::FlexItem (visualiser_panel_).withFlex (1.f));
    right_layout.items.add (LookAndFeel::kFlexSpacer);
    right_layout.items.add (juce::FlexItem (io_panel_).withFlex (1.f));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::row;

    layout.items.add (juce::FlexItem (ir_engine_panel_).withFlex (3.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (right_layout).withFlex (4.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void EditorComponent::RenderFinished (IrGraphState state,
                                      IrGraphProcessor::BoxedBuffer render_result)
{
    juce::dsp::AudioBlock<const float> render_block {render_result.get ()};
    //     graph3d_component_.SetAudioBlock (render_block);
}
