zones_add_module(
        MODULE
        ir_repository
        SOURCES
        src/ir_repository/IrRepositoryModel.h
        src/ir_repository/IrRepositoryAction.h
        src/ir_repository/IrRepositoryAction.cpp
        src/ir_repository/effect/LoadIrEffect.h
        src/ir_repository/effect/LoadIrEffect.cpp
        src/ir_repository/effect/RefreshUserIrsEffect.h
        src/ir_repository/effect/RefreshUserIrsEffect.cpp
        DEPS
        juce::juce_audio_basics
        juce::juce_audio_formats
        lager
        immer
        ir_engine
        ir_format)

zones_add_test(
        MODULE
        ir_repository
        SOURCES
        test/project/ProjectIrPathsTests.cpp
        test/project/ProjectIrRepositoryActionTests.cpp
        test/project/ProjectIrImportControllerTests.cpp
        test/project/ProjectIrLoadControllerTests.cpp
        test/project/ReaderWriterMocks.h
        test/project/LagerTestUtils.h
        DEPS
        ir_repository)

target_compile_definitions(
        ir_repository_test
        PUBLIC "TEST_DATA_DIRECTORY=\"${CMAKE_CURRENT_SOURCE_DIR}/test_data/\"")
