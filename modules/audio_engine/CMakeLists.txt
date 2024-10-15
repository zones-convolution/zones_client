zones_add_module(
        MODULE
        audio_engine
        SOURCES
        src/audio_engine/AudioEngine.h
        src/audio_engine/AudioEngine.cpp
        src/audio_engine/AudioGraph.h
        src/audio_engine/AudioGraph.cpp
        src/audio_engine/VisitorQueueT.h
        src/audio_engine/CommandQueue.h
        src/audio_engine/NotificationQueue.h
        src/audio_engine/AudioGraphMetering.h
        src/audio_engine/AudioGraphMetering.cpp
        src/audio_engine/player/PlayerController.h
        src/audio_engine/player/PlayerController.cpp
        src/audio_engine/player/PlayerProcessor.h
        src/audio_engine/player/PlayerProcessor.cpp
        src/audio_engine/player/PlayerState.h
        src/audio_engine/player/PlayerState.cpp
        src/audio_engine/EQProcessor.h
        src/audio_engine/EQProcessor.cpp
        DEPS
        model
        format
        choc
        ir_engine
        zones_convolver
        juce::juce_core
        juce::juce_dsp
        juce::juce_audio_basics)

embed_resources(audio_engine resources/snare.mp3 resources/numbers.mp3)

zones_add_test(MODULE audio_engine SOURCES
        test/VisitorQueueTests.cpp
        DEPS
        audio_engine
        melatonin_test_helpers
        melatonin_audio_sparklines)

target_compile_definitions(
        audio_engine_test
        PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
)
