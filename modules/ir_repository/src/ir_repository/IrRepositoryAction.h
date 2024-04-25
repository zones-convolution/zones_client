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
    immer::flex_vector<std::filesystem::path> search_paths;
};

struct RefreshUserIrsResultAction
{
    immer::flex_vector<IrMetadata> user_irs;
};

struct LoadIrAction
{
    immer::flex_vector<std::filesystem::path> search_paths;
    std::filesystem::path ir_path;
    TargetFormat target_format;
};

struct LoadIrSuccessAction
{
    std::filesystem::path ir_path;
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
