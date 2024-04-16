#pragma once

#include "account/AccountModel.h"
#include "ir_repository/project/IrLoadingModel.h"

struct Model
{
    IrLoadingModel ir_loading_model;
    AccountModel account_model;
};