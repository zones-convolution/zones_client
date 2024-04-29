zones_add_module(
        MODULE
        zone_format
        SOURCES
        src/zone_format/ZoneMetadata.h
        src/zone_format/ZoneMetadata.cpp
        src/zone_format/JsonUtility.h
        DEPS
        ir_format
        nlohmann_json::nlohmann_json
)

zones_add_test(
        MODULE
        zone_format
        SOURCES
        test/ZoneMetadataTests.cpp
        DEPS
        zone_format
        juce::juce_core)

target_compile_definitions(
        zone_format
        PUBLIC "TEST_DATA_DIRECTORY=\"${CMAKE_CURRENT_SOURCE_DIR}/test_data/\"")
