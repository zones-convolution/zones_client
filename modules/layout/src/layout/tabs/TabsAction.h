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
    virtual ~TabsControllerDelegate () = default;
    virtual bool LoadTab (const std::string & tab_name, bool animate) = 0;
};

using TabsAction = std::variant<LoadTabAction, TabLoadedAction>;
using TabsDeps = lager::deps<TabsControllerDelegate &>;
using TabsResult = lager::result<TabsModel, TabsAction, TabsDeps>;

TabsResult UpdateTabs (TabsModel tabs_model, TabsAction tabs_action);