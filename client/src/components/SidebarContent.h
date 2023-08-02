#pragma once
#include "layout/tabs/TabsAction.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class SidebarContent : public juce::Component
{
public:
    explicit SidebarContent (lager::context<TabsAction> tabs_context);
    void resized () override;

private:
    juce::TextButton browse_button_ {"Browse"};
    juce::TextButton edit_button_ {"Edit"};
    juce::TextButton settings_button_ {"Settings"};

    lager::context<TabsAction> tabs_context_;
};
