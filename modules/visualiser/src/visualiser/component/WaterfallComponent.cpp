#include "WaterfallComponent.h"

WaterfallComponent::WaterfallComponent ()
{
    SetupOpenGl ();

    addAndMakeVisible (status_label_);
    status_label_.setJustificationType (juce::Justification::topLeft);

    refresh_button_.onClick = [&] () { waterfall_renderer_.UpdateShaders (); };
    waterfall_renderer_.UpdateShaders ();

    scale_slider_.setRange ({0.f, 1.f}, 0.f);
    scale_slider_.setValue (1.f);
    scale_slider_.onValueChange = [&] ()
    { waterfall_renderer_.scale_ = (float) scale_slider_.getValue (); };

    offset_x_slider_.setRange ({-1.f, 1.f}, 0.f);
    offset_x_slider_.setValue (0.f);
    offset_x_slider_.onValueChange = [&] ()
    { waterfall_renderer_.offset_x_ = (float) offset_x_slider_.getValue (); };

    offset_y_slider_.setRange ({-1.f, 1.f}, 0.f);
    offset_y_slider_.setValue (0.f);
    offset_y_slider_.onValueChange = [&] ()
    { waterfall_renderer_.offset_y_ = (float) offset_y_slider_.getValue (); };

    addAndMakeVisible (show_time_grid_);
    addAndMakeVisible (show_frequency_grid_);
    colour_scheme_.addItemList ({"Parula",
                                 "Heat",
                                 "Jet",
                                 "Turbo",
                                 "Hot",
                                 "Gray",
                                 "Magma",
                                 "Inferno",
                                 "Plasma",
                                 "Viridis",
                                 "Cividis",
                                 "Github",
                                 "Cubehelix",
                                 "HSV"},
                                1);
    addAndMakeVisible (colour_scheme_);

    colour_scheme_.setSelectedId (0);
    show_time_grid_.setToggleState (true, juce::dontSendNotification);
    show_frequency_grid_.setToggleState (false, juce::dontSendNotification);

    show_time_grid_.onStateChange = [&] () { ParametersUpdated (); };
    show_frequency_grid_.onStateChange = [&] () { ParametersUpdated (); };
    colour_scheme_.onChange = [&] () { ParametersUpdated (); };

    ParametersUpdated ();
}

WaterfallComponent::~WaterfallComponent ()
{
    open_gl_context_.setContinuousRepainting (false);
    open_gl_context_.detach ();
}

void WaterfallComponent::ParametersUpdated ()
{
    tinycolormap::ColormapType colour_scheme {colour_scheme_.getSelectedItemIndex ()};
    waterfall_renderer_.LoadParameters (
        {.draw_time_grid = show_time_grid_.getToggleState (),
         .draw_frequency_grid = show_frequency_grid_.getToggleState (),
         .colour_scheme = colour_scheme});
}

void WaterfallComponent::SetupOpenGl ()
{
    setOpaque (true);
    open_gl_context_.setOpenGLVersionRequired (juce::OpenGLContext::openGL4_1);

    if (auto * peer = getPeer ())
        peer->setCurrentRenderingEngine (0);

    open_gl_context_.setComponentPaintingEnabled (true);
    open_gl_context_.setRenderer (&waterfall_renderer_);
    open_gl_context_.attachTo (*this);
    open_gl_context_.setContinuousRepainting (true);
}

void WaterfallComponent::resized ()
{
    draggable_orientation_.SetBounds (getLocalBounds ());

    juce::FlexBox parameters_layout;
    parameters_layout.flexDirection = juce::FlexBox::Direction::row;
    parameters_layout.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    parameters_layout.items.add (juce::FlexItem (show_time_grid_).withWidth (100.f));
    parameters_layout.items.add (LookAndFeel::kFlexSpacer);
    parameters_layout.items.add (juce::FlexItem (show_frequency_grid_).withWidth (140.f));
    parameters_layout.items.add (LookAndFeel::kFlexSpacer);
    parameters_layout.items.add (juce::FlexItem (colour_scheme_).withFlex (1.f));

    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;
    layout.justifyContent = juce::FlexBox::JustifyContent::flexEnd;

    layout.items.add (juce::FlexItem (offset_y_slider_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (offset_x_slider_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (scale_slider_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (status_label_).withHeight (20.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (refresh_button_).withHeight (40.f));
    layout.items.add (LookAndFeel::kFlexSpacer);
    layout.items.add (juce::FlexItem (parameters_layout).withHeight (40.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

void WaterfallComponent::mouseDown (const juce::MouseEvent & event)
{
    draggable_orientation_.MouseDown (event.getPosition ().toFloat ());
}

void WaterfallComponent::mouseDrag (const juce::MouseEvent & event)
{
    draggable_orientation_.MouseDrag (event.getPosition ().toFloat ());
}

void WaterfallComponent::paint (juce::Graphics & g)
{
    juce::ignoreUnused (g);
}

void WaterfallComponent::SetAudioBlock (const juce::dsp::AudioBlock<const float> audio_block)
{
    waterfall_renderer_.SetupGraphTexture (audio_block);
}

void WaterfallComponent::EnableDebugControls (bool enabled)
{
    if (enabled)
    {
        addAndMakeVisible (refresh_button_);
        addAndMakeVisible (scale_slider_);
        addAndMakeVisible (offset_x_slider_);
        addAndMakeVisible (offset_y_slider_);
    }
    else
    {
        removeChildComponent (&refresh_button_);
        removeChildComponent (&scale_slider_);
        removeChildComponent (&offset_x_slider_);
        removeChildComponent (&offset_y_slider_);
    }
}
