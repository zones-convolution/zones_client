zones_add_module(
        MODULE
        model
        SOURCES
        src/model/ParameterTree.h
        src/model/ParameterTree.cpp
        src/model/Model.h
        src/model/Action.h
        src/model/Action.cpp
        DEPS
        juce::juce_core
        lager
        immer
        audio_engine
        ir_engine
        account
)

zones_add_test(MODULE model SOURCES
        DEPS model)
