#include "TabsController.h"

TabsController::TabsController (TabsComponent & tabs_component)
    : tabs_component_ (tabs_component)
{
}

bool TabsController::LoadTab (const std::string & tab_name, bool animate)
{
    if (! tabs_.contains (tab_name))
    {
        jassertfalse;
        return false;
    }

    tabs_component_.DisplayTab (tabs_.at (tab_name));
    return true;
}
void TabsController::AddTab (const std::string & tab_name, juce::Component & component)
{
    jassert (! tabs_.contains (tab_name));
    tabs_.emplace (tab_name, std::reference_wrapper<juce::Component> (component));
}
