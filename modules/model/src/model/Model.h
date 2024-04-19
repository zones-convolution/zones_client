#pragma once

#include "account/AccountModel.h"
#include "ir_repository/IrLoadingModel.h"
#include "ir_repository/IrRepositoryModel.h"

struct Model
{
    IrRepositoryModel ir_repository_model;
    IrLoadingModel ir_loading_model;
    AccountModel account_model;
};