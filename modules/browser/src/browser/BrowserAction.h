#pragma once

#include "BrowserModel.h"

#include <immer/flex_vector.hpp>
#include <lager/effect.hpp>
#include <variant>

struct LoadHomeAction
{
};

struct LoadZoneAction
{
    ZoneMetadata zone_metadata;
};

struct LoadTop10Action
{
};

struct PopAction
{
};

struct JumpAction
{
    int position;
};

using BrowserAction =
    std::variant<LoadHomeAction, LoadZoneAction, LoadTop10Action, PopAction, JumpAction>;
using BrowserResult = lager::result<BrowserModel, BrowserAction, lager::deps<>>;
using BrowserContext = BrowserResult::effect_t::context_t;

BrowserResult UpdateBrowser (BrowserModel model, BrowserAction action);
