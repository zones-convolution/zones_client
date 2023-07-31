#pragma once

#include "TabsModel.h"

#include <lager/context.hpp>
#include <lager/effect.hpp>
#include <lager/util.hpp>
#include <variant>

struct LoadTabAction
{
    std::string tab_name;
};

struct TabLoadedAction
{
    std::string tab_name;
};

struct TabsControllerDelegate
{
    virtual bool LoadTab (const std::string & tab_name, bool animate) = 0;
};

using TabsAction = std::variant<LoadTabAction, TabLoadedAction>;
using TabsEffect = lager::effect<TabsAction, lager::deps<TabsControllerDelegate &>>;
using TabsResult = std::pair<TabsModel, TabsEffect>;

TabsResult UpdateTabs (TabsModel tabs_model, TabsAction tabs_action);