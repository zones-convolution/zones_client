#pragma once

#pragma once

#include "IrRepositoryModel.h"
#include "ir_engine/IrController.h"

#include <filesystem>
#include <immer/vector.hpp>
#include <lager/effect.hpp>
#include <variant>

struct RefreshUserIrsAction
{
};

struct RefreshUserIrsResultAction
{
    immer::flex_vector<IrMetadata> user_irs;
};

struct LoadIrAction
{
    IrMetadata ir_metadata;
    TargetFormat target_format;
};

struct LoadIrSuccessAction
{
    IrMetadata ir_metadata;
};

struct LoadIrFailureAction
{
};

using IrRepositoryAction = std::variant<RefreshUserIrsAction,
                                        RefreshUserIrsResultAction,
                                        LoadIrAction,
                                        LoadIrSuccessAction,
                                        LoadIrFailureAction>;
using IrRepositoryResult =
    lager::result<IrRepositoryModel, IrRepositoryAction, lager::deps<IrController &>>;
using IrRepositoryContext = IrRepositoryResult::effect_t::context_t;

IrRepositoryResult UpdateIrRepository (IrRepositoryModel model, IrRepositoryAction action);
