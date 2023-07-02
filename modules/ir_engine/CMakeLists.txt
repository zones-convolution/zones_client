zones_add_module(
        MODULE
        ir_engine
        SOURCES
        src/ir_engine/IrEngine.cpp
        src/ir_engine/IrEngine.h
        src/ir_engine/IrGraph.cpp
        src/ir_engine/IrGraph.h
        src/ir_engine/processors/TestProcessor.h
        src/ir_engine/processors/TestProcessor.cpp
        src/ir_engine/IrGraphHashing.cpp
        src/ir_engine/IrGraphHashing.h
        src/ir_engine/IrGraphProcessor.h
        DEPS
        juce::juce_core
        juce::juce_dsp)

zones_add_test(MODULE ir_engine SOURCES
        test/IrEngineTests.cpp
        test/IrGraphHashingTests.cpp
        DEPS ir_engine)
