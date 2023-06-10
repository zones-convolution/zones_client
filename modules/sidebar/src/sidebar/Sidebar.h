#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
namespace zones
{
class Sidebar : public juce::Component
{
public:
    Sidebar ();

private:
    void paint (juce::Graphics & graphics) override;
};
}
