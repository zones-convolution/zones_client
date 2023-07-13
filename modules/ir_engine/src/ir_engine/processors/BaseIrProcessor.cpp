#include "BaseIrProcessor.h"

void BaseIrProcessor::Process (IrGraphProcessor::BoxedBuffer & input_buffer,
                               juce::AudioBuffer<float> & output_buffer,
                               const IrGraphState & state)
{
    output_buffer.makeCopyOf (state.base_ir_buffer.get ());
}