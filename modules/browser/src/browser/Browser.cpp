#include "Browser.h"

namespace zones
{
Browser::Browser ()
    : juce::WindowsWebView2WebBrowserComponent (true, juce::WebView2Preferences ())
{
    goToURL ("https://zones-convolution.vercel.app");
}
void Browser::mouseDown (const juce::MouseEvent & event)
{
    if(event.mods.isRightButtonDown())
        return;

    Component::mouseDown (event);
}
}