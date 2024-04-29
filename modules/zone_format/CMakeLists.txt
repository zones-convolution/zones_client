zones_add_module(
        MODULE
        zone_format
        SOURCES
        src/zone_format/ZoneMetadata.h
        src/zone_format/ZoneMetadata.cpp
        DEPS
        nlohmann_json::nlohmann_json
)

zones_add_test(
        MODULE
        zone_format
        SOURCES

        DEPS
        zone_format)