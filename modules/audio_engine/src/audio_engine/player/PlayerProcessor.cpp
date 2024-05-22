#include "PlayerProcessor.h"

#include <juce_core/juce_core.h>

extern "C" const char resources_snare_mp3 [];
extern "C" const unsigned resources_snare_mp3_size;

static const juce::MemoryInputStream snare_stream {&resources_snare_wav,
                                                   resources_snare_wav_size,
                                                   false};
