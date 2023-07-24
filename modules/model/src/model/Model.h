#pragma once

#include "ParameterModel.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

struct Model
{
    BoxedParameterModel parameter_model;
    ProjectIrRepositoryModel project_ir_repository_model;
};