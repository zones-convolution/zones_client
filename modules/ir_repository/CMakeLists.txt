zones_add_module(
        MODULE
        ir_repository
        SOURCES
        src/ir_repository/io/IrReader.h
        src/ir_repository/io/IrReader.cpp
        src/ir_repository/io/IrWriter.h
        src/ir_repository/io/IrWriter.cpp
        src/ir_repository/io/ScopedIrIO.h
        src/ir_repository/io/ScopedIrIO.cpp
        src/ir_repository/IrData.h
        src/ir_repository/IrMetadata.h
        src/ir_repository/IrMetadata.cpp
        DEPS
        juce::juce_audio_basics
        juce::juce_audio_formats
        lager)

zones_add_test(
        MODULE
        ir_repository
        SOURCES
        test/io/IrReaderTests.cpp
        test/IrMetdataTests.cpp
        DEPS
        ir_repository)

target_compile_definitions(ir_repository_test PUBLIC "TEST_DATA_DIRECTORY=\"${CMAKE_CURRENT_SOURCE_DIR}/test_data/\"")