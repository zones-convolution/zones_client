#pragma once
#include "ir_engine/IrController.h"

#include <juce_core/juce_core.h>

class WebLoadController
{
public:
    WebLoadController (juce::ThreadPool & thread_pool, IrController & ir_controller);

private:
};
