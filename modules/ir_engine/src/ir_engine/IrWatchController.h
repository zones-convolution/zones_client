#pragma once
#include "IrEngine.h"
#include "ir_repository/project/ProjectIrLoadController.h"
#include "ir_repository/project/ProjectIrRepositoryModel.h"

#include <lager/reader.hpp>

class IrWatchController
{
public:
    IrWatchController (IrEngine & ir_engine,
                       ProjectIrLoadController & load_controller,
                       lager::reader<CurrentProjectIrOptional> current_ir_reader);

private:
    IrEngine & ir_engine_;
    ProjectIrLoadController & load_controller_;
    lager::reader<CurrentProjectIrOptional> current_ir_reader_;
};
