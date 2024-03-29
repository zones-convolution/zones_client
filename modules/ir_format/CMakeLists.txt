zones_add_module(
        MODULE
        ir_format
        SOURCES
        src/ir_format/IrMetadata.h
        src/ir_format/IrMetadata.cpp
        src/ir_format/IrFormatter.h
        src/ir_format/IrFormatter.cpp
        src/ir_format/formatter/StereoFormatter.h
        src/ir_format/formatter/StereoFormatter.cpp
        src/ir_format/io/IrReader.h
        src/ir_format/io/IrReader.cpp
        src/ir_format/io/IrWriter.h
        src/ir_format/io/IrWriter.cpp
        src/ir_format/IrData.h
        src/ir_format/IrData.cpp
        DEPS
        juce::juce_audio_basics
        juce::juce_audio_formats
        immer)

zones_add_test(
        MODULE
        ir_format
        SOURCES
        test/IrMetadataTests.cpp
        test/io/IrReaderTests.cpp
        test/io/IrWriterTests.cpp
        DEPS
        ir_format)

target_compile_definitions(
        ir_format_test
        PUBLIC "TEST_DATA_DIRECTORY=\"${CMAKE_CURRENT_SOURCE_DIR}/test_data/\"")
