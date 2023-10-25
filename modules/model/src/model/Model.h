#pragma once

#include "account/AccountModel.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

struct Model
{
    ProjectIrRepositoryModel project_ir_repository_model;
    AccountModel account_model;
};