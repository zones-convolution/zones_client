#include "VisualiserRelay.h"

VisualiserRelay::VisualiserRelay (juce::WebBrowserComponent & web_browser_component,
                                  IrEngine & ir_engine)
    : ir_engine_ (ir_engine)
{
    ir_engine_.GetListeners ().add (this);
}

VisualiserRelay::~VisualiserRelay ()
{
    ir_engine_.GetListeners ().remove (this);
}

juce::WebBrowserComponent::Options
VisualiserRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return juce::WebBrowserComponent::Options ();
}

void VisualiserRelay::RenderFinished (IrGraphState state,
                                      IrGraphProcessor::BoxedBuffer render_result)
{
}
