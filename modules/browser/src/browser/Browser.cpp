#include "Browser.h"

namespace zones
{
Browser::Browser ()
    : juce::WebBrowserComponent ()
{
    goToURL ("https://zones-convolution.vercel.app");
}
void Browser::mouseDown (const juce::MouseEvent & event)
{
    if (event.mods.isRightButtonDown ())
        return;

    Component::mouseDown (event);
}
}