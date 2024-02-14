zones_add_module(
        MODULE
        audio_engine
        SOURCES
        src/audio_engine/AudioEngine.h
        src/audio_engine/AudioEngine.cpp
        src/audio_engine/AudioGraph.h
        src/audio_engine/AudioGraph.cpp
        src/audio_engine/VisitorQueue.h
        src/audio_engine/CommandQueue.h
        src/audio_engine/AudioGraphMetering.h
        src/audio_engine/AudioGraphMetering.cpp
        DEPS
        model
        ir_repository
        choc
        zones_convolver
        juce::juce_core
        juce::juce_dsp
        juce::juce_audio_basics)

zones_add_test(MODULE audio_engine SOURCES
        test/VisitorQueueTests.cpp
        DEPS
        audio_engine
        melatonin_test_helpers
        melatonin_audio_sparklines)
