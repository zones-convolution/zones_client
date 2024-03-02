#pragma once

#include "Model.h"
#include "account/AccountAction.h"
#include "ir_engine/IrController.h"
#include "ir_repository/project/ProjectIrRepositoryAction.h"

#include <variant>

using Action = std::variant<ProjectIrRepositoryAction, AccountAction>;
using Deps = lager::deps<IrController &>;
using Result = lager::result<Model, Action, Deps>;
Result Update (Model model, Action action);