#pragma once
#include "spectrogram_visualiser/RingBuffer.h"
#include "spectrogram_visualiser/SpectrogramVisualiserComponent.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class Visualisers : public juce::Component
{
public:
    Visualisers ();
    void resized () override;

private:
    juce::Label visualisers_label_;
    RingBuffer<GLfloat> ring_buffer_ {2, 1024};
    SpectrogramVisualiserComponent spectrogram_visualiser_component_ {&ring_buffer_};
};
