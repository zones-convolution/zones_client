zones_add_module(
        MODULE
        api
        SOURCES
        src/api/DeviceApi.h
        src/api/DeviceApi.cpp
        src/api/OidcApi.h
        src/api/OidcApi.cpp
        DEPS
        juce::juce_core
        juce::juce_events
        cpr::cpr)

zones_add_test(MODULE api SOURCES test/TestDeviceApi.cpp test/TestOidcApi.cpp DEPS api)
