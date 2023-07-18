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
        DEPS
        ir_repository
        choc
        juce::juce_core
        juce::juce_dsp
        juce::juce_audio_basics)

zones_add_test(MODULE audio_engine SOURCES test/VisitorQueueTests.cpp
        DEPS audio_engine)
