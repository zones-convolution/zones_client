#pragma once

#include "../controllers/visualiser/VisualiserController.h"
#include "ir_engine/IrEngine.h"
#include "juce_gui_extra/juce_gui_extra.h"

#include <immer/box.hpp>

class VisualiserRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    using BoxedBuffer = immer::box<juce::AudioBuffer<float>>;

    VisualiserRelay (juce::WebBrowserComponent & web_browser_component,
                     VisualiserController & visualiser_controller);
    ~VisualiserRelay () override;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    JUCE_DECLARE_WEAK_REFERENCEABLE (VisualiserRelay)
};