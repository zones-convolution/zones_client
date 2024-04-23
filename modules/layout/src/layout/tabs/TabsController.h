#pragma once
#include "TabsAction.h"
#include "TabsComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

class TabsController : public TabsControllerDelegate
{
public:
    explicit TabsController (TabsComponent & tabs_component);
    ~TabsController () override = default;
    bool LoadTab (const std::string & tab_name, bool animate) override;
    void AddTab (const std::string & tab_name, juce::Component & component);

private:
    std::unordered_map<std::string, std::reference_wrapper<juce::Component>> tabs_;
    TabsComponent & tabs_component_;
};
