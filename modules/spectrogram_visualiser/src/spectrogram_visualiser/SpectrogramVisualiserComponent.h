#pragma once

#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>

class SpectrogramVisualiserComponent : public juce::Component
{
public:
    SpectrogramVisualiserComponent ();
    ~SpectrogramVisualiserComponent () override;

    void paint (juce::Graphics & g) override;
    void resized () override;

private:
    juce::Label status_label_ {{}, "Status"};
    juce::ComboBox preset_box_;

    juce::String fragment_code_;
    std::unique_ptr<juce::OpenGLGraphicsContextCustomShader> fragment_shader_;
    juce::OpenGLContext open_gl_context_;

    struct ShaderPreset
    {
        const char * name;
        const char * fragment_shader;
    };

    static juce::Array<ShaderPreset> GetPresets ();
    void SelectPreset (int preset);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrogramVisualiserComponent)
};