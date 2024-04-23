#pragma once
#include "../BrowserModel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class Top10ViewComponent : public juce::Component
{
public:
    Top10ViewComponent ();
    void Update (const Top10View & top_10_view);
    void resized () override;

private:
    juce::Label label_ {"Top 10 View", "Top 10 View"};
};
