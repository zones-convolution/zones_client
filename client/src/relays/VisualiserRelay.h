#pragma once

#include "../controllers/visualiser/VisualiserController.h"
#include "ir_engine/IrEngine.h"
#include "juce_gui_extra/juce_gui_extra.h"

#include <immer/box.hpp>

class VisualiserRelay
{
public:
    VisualiserRelay (juce::WebBrowserComponent & web_browser_component,
                     VisualiserController & visualiser_controller);
    ~VisualiserRelay () = default;
    [[nodiscard]] juce::WebBrowserComponent::Resource GetVisualiserResource () const;

private:
    rocket::scoped_connection_container connections_;
    juce::WebBrowserComponent & web_browser_component_;
    VisualiserController & visualiser_controller_;

    JUCE_DECLARE_WEAK_REFERENCEABLE (VisualiserRelay)
};