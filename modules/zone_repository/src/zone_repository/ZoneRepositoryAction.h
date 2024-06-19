#pragma once

#pragma once

#include "ZoneRepositoryModel.h"
#include "format/IrData.h"
#include "ir_engine/IrController.h"

#include <filesystem>
#include <immer/vector.hpp>
#include <lager/effect.hpp>
#include <variant>

struct RefreshValidTargetFormatsAction
{
    std::vector<TargetFormat> target_formats;
};

struct LoadIrAction
{
    IrSelection ir_selection;
};

struct LoadIrSuccessAction
{
    IrSelection ir;
};

struct LoadIrFailureAction
{
};

using ZoneRepositoryAction = std::variant<RefreshValidTargetFormatsAction,
                                          LoadIrAction,
                                          LoadIrSuccessAction,
                                          LoadIrFailureAction>;
using ZoneRepositoryResult =
    lager::result<ZoneRepositoryModel, ZoneRepositoryAction, lager::deps<IrController &>>;

using ZoneRepositoryContext = ZoneRepositoryResult::effect_t::context_t;

ZoneRepositoryResult UpdateZoneRepository (ZoneRepositoryModel model, ZoneRepositoryAction action);
