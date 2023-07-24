zones_add_module(
        MODULE
        model
        SOURCES
        src/model/controllers/IrWatchController.h
        src/model/controllers/IrWatchController.cpp
        DEPS
        juce::juce_core
        lager
        immer
        audio_engine
        ir_engine
)

zones_add_test(MODULE model SOURCES
        DEPS model)
