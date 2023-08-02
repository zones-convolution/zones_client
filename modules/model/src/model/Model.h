#pragma once

#include "ParameterModel.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

struct Model
{
    BoxedParameterModel parameter_model;
    ProjectIrRepositoryModel project_ir_repository_model;

    static auto ParameterReader (lager::reader<Model> reader)
    {
        return reader.zoom (lager::lenses::attr (&Model::parameter_model));
    }

    static auto ProjectIrRepositoryReader (lager::reader<Model> reader)
    {
        return reader.zoom (lager::lenses::attr (&Model::project_ir_repository_model));
    }
};