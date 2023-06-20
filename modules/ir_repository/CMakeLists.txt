zones_add_module(
        MODULE ir_repository
        SOURCES src/ir_repository/io/IrReader.h
        src/ir_repository/io/IrReader.cpp
        src/ir_repository/io/IrWriter.h
        src/ir_repository/io/IrWriter.cpp
        src/ir_repository/io/ScopedIrIO.h
        src/ir_repository/io/ScopedIrIO.cpp
        src/ir_repository/IrData.h
        DEPS juce::juce_audio_basics lager
)

zones_add_test(
        MODULE ir_repository
        SOURCES test/TestTestModuleClass.cpp
        DEPS ir_repository
)