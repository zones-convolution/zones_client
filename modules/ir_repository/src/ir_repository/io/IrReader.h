#pragma once

#include "../IrData.h"

#include <optional>
#include <variant>

class IrReader
{
public:
    static IrData ReadIrData (std::string load_path, std::string ir_identifier);
};
