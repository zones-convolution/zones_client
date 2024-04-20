#pragma once

#include "Model.h"
#include "ir_engine/IrController.h"
#include "ir_repository/IrLoadingAction.h"
#include "ir_repository/IrRepositoryAction.h"

#include <variant>

using Action = std::variant<IrRepositoryAction, IrLoadingAction>;
using Deps = lager::deps<IrController &>;
using Result = lager::result<Model, Action, Deps>;
Result Update (Model model, Action action);