#pragma once

#include "../ProcessorContainer.h"

#include <juce_gui_extra/juce_gui_extra.h>

struct EngineLoading
{
    bool convolution_engine_loading;
    bool ir_engine_loading;
};

class EngineRelay : public OptionsBuilder<juce::WebBrowserComponent::Options>
{
public:
    EngineRelay (juce::WebBrowserComponent & web_browser_component,
                 IrEngine & ir_engine,
                 zones::ConvolutionEngine & convolution_engine);
    ~EngineRelay () override = default;

    juce::WebBrowserComponent::Options
    buildOptions (const juce::WebBrowserComponent::Options & initialOptions) override;

private:
    void OnEngineUpdatedEmitEvent ();
    juce::WebBrowserComponent & web_browser_component_;
    IrEngine & ir_engine_;
    zones::ConvolutionEngine & convolution_engine_;
};
