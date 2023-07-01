zones_add_module(
        MODULE
        ir_engine
        SOURCES
        src/ir_engine/IrEngine.cpp
        src/ir_engine/IrEngine.h
        DEPS
        juce::juce_core)

zones_add_test(MODULE ir_engine SOURCES
        test/IrEngineTests.cpp
        DEPS ir_engine)
