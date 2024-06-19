zones_add_module(
        MODULE
        zone_repository
        SOURCES
        src/zone_repository/ZoneRepositoryModel.h
        src/zone_repository/ZoneRepositoryAction.h
        src/zone_repository/ZoneRepositoryAction.cpp
        src/zone_repository/effect/LoadIrEffect.h
        src/zone_repository/effect/LoadIrEffect.cpp
        DEPS
        juce::juce_audio_basics
        juce::juce_audio_formats
        lager
        immer
        ir_engine
        format
        preferences)

zones_add_test(
        MODULE
        zone_repository
        SOURCES
        test/project/ProjectIrPathsTests.cpp
        test/project/ProjectIrRepositoryActionTests.cpp
        test/project/ProjectIrImportControllerTests.cpp
        test/project/ProjectIrLoadControllerTests.cpp
        test/project/ReaderWriterMocks.h
        test/project/LagerTestUtils.h
        DEPS
        zone_repository)

target_compile_definitions(
        zone_repository_test
        PUBLIC "TEST_DATA_DIRECTORY=\"${CMAKE_CURRENT_SOURCE_DIR}/test_data/\"")
