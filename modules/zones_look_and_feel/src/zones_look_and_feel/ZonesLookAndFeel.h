#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace zones
{
class ZonesLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ZonesLookAndFeel ();
    ~ZonesLookAndFeel () override = default;

private:
};
}