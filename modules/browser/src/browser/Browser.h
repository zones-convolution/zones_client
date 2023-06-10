#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

namespace zones
{
class Browser : public juce::WindowsWebView2WebBrowserComponent
{
public:
    Browser ();

    void mouseDown (const juce::MouseEvent & event) override;
};
}
