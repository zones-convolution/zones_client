#pragma once

#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_opengl/juce_opengl.h>
class SpectrogramVisualiserComponent
    : public juce::Component
    , private juce::CodeDocument::Listener
    , private juce::Timer
{
public:
    SpectrogramVisualiserComponent ()
    {
        setOpaque (true);

        if (auto * peer = getPeer ())
            peer->setCurrentRenderingEngine (0);

        openGLContext.attachTo (*getTopLevelComponent ());

        addAndMakeVisible (statusLabel);
        statusLabel.setJustificationType (juce::Justification::centred);

        auto presets = getPresets ();

        for (int i = 0; i < presets.size (); ++i)
            presetBox.addItem (presets [i].name, i + 1);

        addAndMakeVisible (presetBox);
        presetBox.onChange = [this] { selectPreset (presetBox.getSelectedItemIndex ()); };

        fragmentEditorComp.setOpaque (false);
        fragmentDocument.addListener (this);
        addAndMakeVisible (fragmentEditorComp);

        presetBox.setSelectedItemIndex (0);
    }

    ~SpectrogramVisualiserComponent () override
    {
        openGLContext.detach ();
        shader.reset ();
    }

    void paint (juce::Graphics & g) override
    {
        g.fillCheckerBoard (getLocalBounds ().toFloat (),
                            48.0f,
                            48.0f,
                            juce::Colours::lightgrey,
                            juce::Colours::white);

        if (shader.get () == nullptr || shader->getFragmentShaderCode () != fragmentCode)
        {
            shader.reset ();

            if (fragmentCode.isNotEmpty ())
            {
                shader.reset (new juce::OpenGLGraphicsContextCustomShader (fragmentCode));

                auto result = shader->checkCompilation (g.getInternalContext ());

                if (result.failed ())
                {
                    statusLabel.setText (result.getErrorMessage (), juce::dontSendNotification);
                    shader.reset ();
                }
            }
        }

        if (shader.get () != nullptr)
        {
            statusLabel.setText ({}, juce::dontSendNotification);

            shader->fillRect (g.getInternalContext (), getLocalBounds ());
        }
    }

    void resized () override
    {
        juce::FlexBox layout;
        layout.flexDirection = juce::FlexBox::Direction::column;
        layout.justifyContent = juce::FlexBox::JustifyContent::flexEnd;

        layout.items.add (juce::FlexItem (statusLabel).withHeight (20.f));
        layout.items.add (LookAndFeel::kFlexSpacer);
        layout.items.add (juce::FlexItem (presetBox).withHeight (40.f));
        layout.items.add (LookAndFeel::kFlexSpacer);
        layout.items.add (juce::FlexItem (fragmentEditorComp).withHeight (80.f));

        layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
    }

    void selectPreset (int preset)
    {
        fragmentDocument.replaceAllContent (getPresets () [preset].fragmentShader);
        startTimer (1);
    }

    std::unique_ptr<juce::OpenGLGraphicsContextCustomShader> shader;

    juce::Label statusLabel {{}, "Shader Preset:"};
    juce::ComboBox presetBox;
    juce::CodeDocument fragmentDocument;
    juce::CodeEditorComponent fragmentEditorComp {fragmentDocument, nullptr};
    juce::String fragmentCode;

private:
    juce::OpenGLContext openGLContext;

    enum
    {
        shaderLinkDelay = 500
    };

    void codeDocumentTextInserted (const juce::String & /*newText*/, int /*insertIndex*/) override
    {
        startTimer (shaderLinkDelay);
    }

    void codeDocumentTextDeleted (int /*startIndex*/, int /*endIndex*/) override
    {
        startTimer (shaderLinkDelay);
    }

    void timerCallback () override
    {
        stopTimer ();
        fragmentCode = fragmentDocument.getAllContent ();
        repaint ();
    }

    struct ShaderPreset
    {
        const char * name;
        const char * fragmentShader;
    };

    static juce::Array<ShaderPreset> getPresets ()
    {
#define SHADER_2DDEMO_HEADER                                                  \
    "/*  This demo shows the use of the OpenGLGraphicsContextCustomShader,\n" \
    "    which allows a 2D area to be filled using a GL shader program.\n"    \
    "\n"                                                                      \
    "    Edit the shader program below and it will be \n"                     \
    "    recompiled in real-time!\n"                                          \
    "*/\n\n"

        ShaderPreset presets [] = {
            {"Simple Gradient",

             SHADER_2DDEMO_HEADER "void main()\n"
                                  "{\n"
                                  "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (1.0, 0.4, 0.6, 1.0);\n"
                                  "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);\n"
                                  "    " JUCE_MEDIUMP " float alpha = pixelPos.x / 1000.0;\n"
                                  "    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);\n"
                                  "}\n"},

            {"Circular Gradient",

             SHADER_2DDEMO_HEADER
             "void main()\n"
             "{\n"
             "    " JUCE_MEDIUMP " vec4 colour1 = vec4 (1.0, 0.4, 0.6, 1.0);\n"
             "    " JUCE_MEDIUMP " vec4 colour2 = vec4 (0.3, 0.4, 0.4, 1.0);\n"
             "    " JUCE_MEDIUMP
             " float alpha = distance (pixelPos, vec2 (600.0, 500.0)) / 400.0;\n"
             "    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);\n"
             "}\n"},

            {"Circle",

             SHADER_2DDEMO_HEADER
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

             SHADER_2DDEMO_HEADER "void main()\n"
                                  "{\n"
                                  "    gl_FragColor = vec4 (1.0, 0.6, 0.1, pixelAlpha);\n"
                                  "}\n"}};

        return juce::Array<ShaderPreset> (presets, juce::numElementsInArray (presets));
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrogramVisualiserComponent)
};