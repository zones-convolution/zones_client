#pragma once

#include "Model.h"
#include "ir_engine/IrController.h"
#include "zone_repository/ZoneRepositoryAction.h"

#include <variant>

using Action = std::variant<ZoneRepositoryAction>;
using Deps = lager::deps<IrController &>;
using Result = lager::result<Model, Action, Deps>;
Result Update (Model model, Action action);