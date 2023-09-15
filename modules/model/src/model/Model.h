#pragma once

#include "ir_repository/project/ProjectIrRepositoryModel.h"

struct Model
{
    ProjectIrRepositoryModel project_ir_repository_model;

    static auto ProjectIrRepositoryReader (lager::reader<Model> reader)
    {
        return reader.zoom (lager::lenses::attr (&Model::project_ir_repository_model));
    }
};