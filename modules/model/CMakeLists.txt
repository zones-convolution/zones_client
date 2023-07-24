zones_add_module(
        MODULE
        model
        SOURCES
        src/model/controllers/IrWatchController.h
        src/model/controllers/IrWatchController.cpp
        src/model/ParameterModel.h
        src/model/ParameterAction.h
        src/model/ParameterAction.cpp
        src/model/Model.h
        src/model/Action.h
        src/model/Action.cpp
        DEPS
        juce::juce_core
        lager
        immer
        audio_engine
        ir_engine
)

zones_add_test(MODULE model SOURCES
        DEPS model)
