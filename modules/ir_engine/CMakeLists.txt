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
        src/ir_engine/ProcessResultPool.h
        src/ir_engine/ProcessResultPool.cpp
        src/ir_engine/IrGraphProcessor.h
        src/ir_engine/IrGraphCachePolicy.h
        src/ir_engine/IrGraphCachePolicy.cpp
        src/ir_engine/IrGraphStateKey.h
        src/ir_engine/IrGraphStateKey.cpp
        DEPS
        immer
        juce::juce_core
        juce::juce_dsp)

zones_add_test(MODULE ir_engine SOURCES
        test/IrGraphCachePolicyTests.cpp
        test/IrGraphTests.cpp
        test/IrEngineTests.cpp
        test/IrGraphStateKeyTests.cpp
        DEPS ir_engine)
