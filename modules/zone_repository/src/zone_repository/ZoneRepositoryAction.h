#pragma once

#pragma once

#include "ZoneRepositoryModel.h"
#include "format/IrData.h"
#include "ir_engine/IrController.h"

#include <filesystem>
#include <immer/vector.hpp>
#include <lager/effect.hpp>
#include <variant>

struct RefreshUserZonesAction
{
};

struct RefreshUserZonesResultAction
{
    immer::flex_vector<ZoneMetadata> user_zones;
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

using ZoneRepositoryAction = std::variant<RefreshUserZonesAction,
                                          RefreshUserZonesResultAction,
                                          LoadIrAction,
                                          LoadIrSuccessAction,
                                          LoadIrFailureAction>;
using ZoneRepositoryResult =
    lager::result<ZoneRepositoryModel, ZoneRepositoryAction, lager::deps<IrController &>>;

using ZoneRepositoryContext = ZoneRepositoryResult::effect_t::context_t;

ZoneRepositoryResult UpdateZoneRepository (ZoneRepositoryModel model, ZoneRepositoryAction action);
