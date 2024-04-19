#pragma once

#pragma once

#include "IrRepositoryModel.h"
#include "ir_engine/IrController.h"

#include <filesystem>
#include <lager/effect.hpp>
#include <variant>

struct RefreshUserIrsAction
{
    immer::flex_vector<std::filesystem::path> search_paths;
};

struct RefreshUserIrsResultAction
{
    immer::flex_vector<IrMetadata> user_irs;
};

using IrRepositoryAction = std::variant<RefreshUserIrsAction, RefreshUserIrsResultAction>;
using IrRepositoryResult = lager::result<IrRepositoryModel, IrRepositoryAction, lager::deps<>>;
using IrRepositoryContext = IrRepositoryResult::effect_t::context_t;

IrRepositoryResult UpdateIrRepository (IrRepositoryModel model, IrRepositoryAction action);
