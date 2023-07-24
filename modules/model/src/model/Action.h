#pragma once

#include "Model.h"
#include "ParameterAction.h"
#include "ir_repository/project/ProjectIrRepositoryAction.h"

#include <lager/util.hpp>
#include <variant>

using Action = std::variant<ParameterAction, ProjectIrRepositoryAction>;

Model Update (Model model, Action action);