#pragma once

#include "ParameterModel.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

struct Model
{
    BoxedIrEngineParameterModel ir_engine_parameter_model;
    BoxedRealtimeParameterModel realtime_parameter_model;
    ProjectIrRepositoryModel project_ir_repository_model;

    static auto IrEngineParameterReader (lager::reader<Model> reader)
    {
        return reader.zoom (lager::lenses::attr (&Model::ir_engine_parameter_model));
    }
    
    static auto RealtimeParameterReader (lager::reader<Model> reader)
    {
        return reader.zoom (lager::lenses::attr (&Model::realtime_parameter_model));
    }

    static auto ProjectIrRepositoryReader (lager::reader<Model> reader)
    {
        return reader.zoom (lager::lenses::attr (&Model::project_ir_repository_model));
    }
};