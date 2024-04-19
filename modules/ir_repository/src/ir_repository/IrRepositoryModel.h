#pragma once

#include "ir_format/IrMetadata.h"

#include <immer/flex_vector.hpp>

struct IrRepositoryModel
{
    bool user_irs_loading = false;
    immer::flex_vector<IrMetadata> user_irs;
};