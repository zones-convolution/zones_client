#pragma once
#include "layout/tabs/TabsAction.h"
#include "zones_look_and_feel/BoxIcons.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/IconTextButton.h"

#include <juce_gui_basics/juce_gui_basics.h>

class SidebarContent : public juce::Component
{
public:
    explicit SidebarContent (lager::context<TabsAction> tabs_context);
    void resized () override;

private:
    IconTextButton browse_button_ {"Browse", BoxIcons::kBxSearch};
    IconTextButton edit_button_ {"Edit", BoxIcons::kBxEdit};
    IconTextButton settings_button_ {"Settings", BoxIcons::kBxCog};

    lager::context<TabsAction> tabs_context_;
};
