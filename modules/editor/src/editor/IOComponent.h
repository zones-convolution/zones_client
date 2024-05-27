#include "PlayerComponent.h"
#include "audio_engine/player/PlayerController.h"
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/DividerComponent.h"
#include "look_and_feel/components/IconButton.h"
#include "look_and_feel/components/PanelComponent.h"
#include "model/ParameterTree.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

class IOComponent : public juce::Component
{
public:
    explicit IOComponent (juce::AudioProcessorValueTreeState & parameter_tree,
                          PlayerController & player_controller);
    void resized () override;

private:
    static const std::string kIOPanelKey;

    void SetPlayerState ();

    juce::Label io_label_;
    DividerComponent top_divider_;

    PlayerComponent player_component_;
    PanelComponent player_panel_ {
        player_component_,
        PanelComponent::ColourPair {juce::Colours::darkblue, juce::Colours::darkblue}};

    juce::Label dry_wet_label_;
    juce::Slider dry_wet_mix_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                      juce::Slider::TextEntryBoxPosition::NoTextBox};
    ParameterTree::SliderAttachment dry_wet_mix_attachment_;

    juce::Label input_gain_label_;
    juce::Slider input_gain_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                     juce::Slider::TextEntryBoxPosition::NoTextBox};
    ParameterTree::SliderAttachment input_gain_attachment_;

    juce::Label output_gain_label_;
    juce::Slider output_gain_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                      juce::Slider::TextEntryBoxPosition::NoTextBox};
    ParameterTree::SliderAttachment output_gain_attachment_;
};
