#pragma once
#include "../IrGraphProcessor.h"

class RoomSizeProcessor : public IrGraphProcessor
{
public:
    void Process (BoxedBuffer & input_buffer,
                  juce::AudioBuffer<float> & output_buffer,
                  const IrGraphState & state) override;
    
    ~RoomSizeProcessor () override = default;
};
