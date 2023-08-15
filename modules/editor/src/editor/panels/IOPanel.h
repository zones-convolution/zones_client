#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/DividerComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

class IOPanel : public juce::Component
{
public:
    IOPanel ();
    void resized () override;

private:
    static const std::string kIOPanelKey;
    static const std::string kDryWetKey;
    static const std::string kInputGainKey;
    static const std::string kOutputGainKey;

    static juce::FlexBox BuildSliderLabelLayout (juce::Component & slider, juce::Label & label);

    juce::Label io_label_;
    DividerComponent top_divider_;

    juce::Label dry_wet_label_;
    juce::Slider dry_wet_mix_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                      juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Label input_gain_label_;
    juce::Slider input_gain_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                     juce::Slider::TextEntryBoxPosition::NoTextBox};
    juce::Label output_gain_label_;
    juce::Slider output_gain_slider_ {juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                                      juce::Slider::TextEntryBoxPosition::NoTextBox};
};
