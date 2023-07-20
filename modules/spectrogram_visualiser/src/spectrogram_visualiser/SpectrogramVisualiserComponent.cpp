#include "SpectrogramVisualiserComponent.h"

extern "C" const char shaders_solid_colour_frag_glsl [];
extern "C" const unsigned shaders_solid_colour_frag_glsl_size;

extern "C" const char shaders_simple_gradient_frag_glsl [];
extern "C" const unsigned shaders_simple_gradient_frag_glsl_size;

extern "C" const char shaders_circular_gradient_frag_glsl [];
extern "C" const unsigned shaders_circular_gradient_frag_glsl_size;

extern "C" const char shaders_circle_frag_glsl [];
extern "C" const unsigned shaders_circle_frag_glsl_size;

SpectrogramVisualiserComponent::SpectrogramVisualiserComponent ()
{
    setOpaque (true);

    if (auto * peer = getPeer ())
        peer->setCurrentRenderingEngine (0);

    open_gl_context_.attachTo (*getTopLevelComponent ());

    addAndMakeVisible (status_label_);
    status_label_.setJustificationType (juce::Justification::centred);

    auto presets = GetPresets ();
    for (int i = 0; i < presets.size (); ++i)
        preset_box_.addItem (presets [i].name, i + 1);
    addAndMakeVisible (preset_box_);
    preset_box_.onChange = [this] { SelectPreset (preset_box_.getSelectedItemIndex ()); };
    preset_box_.setSelectedItemIndex (0);
}

SpectrogramVisualiserComponent::~SpectrogramVisualiserComponent ()
{
    open_gl_context_.detach ();
    shader.reset ();
}

void SpectrogramVisualiserComponent::paint (juce::Graphics & g)
{
    g.fillCheckerBoard (
        getLocalBounds ().toFloat (), 48.0f, 48.0f, juce::Colours::lightgrey, juce::Colours::white);

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

void SpectrogramVisualiserComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.justifyContent = juce::FlexBox::JustifyContent::flexEnd;

    layout.items.add (juce::FlexItem (status_label_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (preset_box_).withHeight (40.f));

    layout.performLayout (getLocalBounds ().toFloat ().reduced (LookAndFeel::kPadding));
}

juce::Array<SpectrogramVisualiserComponent::ShaderPreset>
SpectrogramVisualiserComponent::GetPresets ()
{
    return {
        ShaderPreset {"Simple Gradient", juce::StringRef (shaders_simple_gradient_frag_glsl)},
        ShaderPreset {"Circular Gradient", juce::StringRef (shaders_circular_gradient_frag_glsl)},
        ShaderPreset {"Circle", juce::StringRef (shaders_circle_frag_glsl)},
        ShaderPreset {"Solid Colour", juce::StringRef (shaders_solid_colour_frag_glsl)}};
}

void SpectrogramVisualiserComponent::SelectPreset (int preset)
{
    fragment_code_ = GetPresets () [preset].fragment_shader;
    repaint ();
}