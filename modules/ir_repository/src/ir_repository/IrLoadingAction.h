#pragma once

#include "IrLoadingModel.h"
#include "ir_engine/IrController.h"

#include <filesystem>
#include <lager/effect.hpp>
#include <variant>

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

using IrLoadingAction = std::variant<LoadIrAction, LoadIrSuccessAction, LoadIrFailureAction>;

using IrLoadingResult = lager::result<IrLoadingModel, IrLoadingAction, lager::deps<IrController &>>;
using IrLoadingContext = IrLoadingResult::effect_t::context_t;

IrLoadingResult UpdateIrLoading (IrLoadingModel model, IrLoadingAction action);
