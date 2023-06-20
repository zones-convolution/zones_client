#pragma once

#include "../IrData.h"

#include <juce_core/juce_core.h>
#include <optional>
#include <variant>

class IrReader
{
public:
    static IrData ReadIrData (std::string load_path, std::string ir_identifier);

private:
};
