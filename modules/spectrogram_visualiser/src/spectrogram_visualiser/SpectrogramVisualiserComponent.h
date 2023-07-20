#pragma once

#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>
class SpectrogramVisualiserComponent : public juce::Component
{
public:
    SpectrogramVisualiserComponent ()
    {
        setOpaque (true);

        if (auto * peer = getPeer ())
            peer->setCurrentRenderingEngine (0);

        open_gl_context_.attachTo (*getTopLevelComponent ());

        addAndMakeVisible (status_label_);
        status_label_.setJustificationType (juce::Justification::centred);

        auto presets = getPresets ();

        for (int i = 0; i < presets.size (); ++i)
            preset_box_.addItem (presets [i].name, i + 1);

        addAndMakeVisible (preset_box_);
        preset_box_.onChange = [this] { selectPreset (preset_box_.getSelectedItemIndex ()); };

        preset_box_.setSelectedItemIndex (0);
    }

    ~SpectrogramVisualiserComponent () override
    {
        open_gl_context_.detach ();
        shader.reset ();
    }

    void paint (juce::Graphics & g) override
    {
        g.fillCheckerBoard (getLocalBounds ().toFloat (),
                            48.0f,
                            48.0f,
                            juce::Colours::lightgrey,
                            juce::Colours::white);

        if (shader.get () == nullptr || shader->getFragmentShaderCode () != fragment_code_)
        {
            shader.reset ();

            if (fragment_code_.isNotEmpty ())
            {
                shader.reset (new juce::OpenGLGraphicsContextCustomShader (fragment_code_));

                auto result = shader->checkCompilation (g.getInternalContext ());

                if (result.failed ())
                {
                    status_label_.setText (result.getErrorMessage (), juce::dontSendNotification);
                    shader.reset ();
                }
            }
        }

        if (shader.get () != nullptr)
        {
            status_label_.setText ({}, juce::dontSendNotification);

            shader->fillRect (g.getInternalContext (), getLocalBounds ());
        }
    }

    void resized () override
    {
        juce::FlexBox layout;
        layout.flexDirection = juce::FlexBox::Direction::column;
        layout.justifyContent = juce::FlexBox::JustifyContent::flexEnd;

        layout.items.add (juce::FlexItem (status_label_).withHeight (20.f));
        layout.items.add (LookAndFeel::kFlexSpacer);
        layout.items.add (juce::FlexItem (preset_box_).withHeight (40.f));

        layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
    }

    void selectPreset (int preset)
    {
        fragment_code_ = getPresets () [preset].fragment_shader;
        repaint ();
    }

    std::unique_ptr<juce::OpenGLGraphicsContextCustomShader> shader;

    juce::Label status_label_ {{}, "Status"};
    juce::ComboBox preset_box_;
    juce::String fragment_code_;

private:
    juce::OpenGLContext open_gl_context_;

    struct ShaderPreset
    {
        const char * name;
        const char * fragment_shader;
    };

    static juce::Array<ShaderPreset> getPresets ()
    {
        ShaderPreset presets [] = {
            {"Simple Gradient",
             "void main()\n"
             "{\n"
             "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (1.0, 0.4, 0.6, 1.0);\n"
             "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);\n"
             "    " JUCE_MEDIUMP " float alpha = pixelPos.x / 1000.0;\n"
             "    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);\n"
             "}\n"},

            {"Circular Gradient",
             "void main()\n"
             "{\n"
             "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (1.0, 0.4, 0.6, 1.0);\n"
             "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.3, 0.4, 0.4, 1.0);\n"
             "    " JUCE_MEDIUMP
             " float alpha = distance (pixelPos, vec2 (600.0, 500.0)) / 400.0;\n"
             "    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);\n"
             "}\n"},

            {"Circle",
             "void main()\n"
             "{\n"
             "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (0.1, 0.1, 0.9, 1.0);\n"
             "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);\n"
             "    " JUCE_MEDIUMP " float distance = distance (pixelPos, vec2 (600.0, 500.0));\n"
             "\n"
             "    " JUCE_MEDIUMP " float innerRadius = 200.0;\n"
             "    " JUCE_MEDIUMP " float outerRadius = 210.0;\n"
             "\n"
             "    if (distance < innerRadius)\n"
             "        gl_FragColor = colour1;\n"
             "    else if (distance > outerRadius)\n"
             "        gl_FragColor = colour2;\n"
             "    else\n"
             "        gl_FragColor = mix (colour1, colour2, (distance - innerRadius) / (outerRadius - innerRadius));\n"
             "\n"
             "    gl_FragColor *= pixelAlpha;\n"
             "}\n"},

            {"Solid Colour",
             "void main()\n"
             "{\n"
             "    gl_FragColor = vec4 (1.0, 0.6, 0.1, pixelAlpha);\n"
             "}\n"}};

        return juce::Array<ShaderPreset> (presets, juce::numElementsInArray (presets));
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrogramVisualiserComponent)
};