zones_add_module(
        MODULE
        api
        SOURCES
        src/api/DeviceApi.h
        src/api/DeviceApi.cpp
        src/api/OidcApi.h
        src/api/OidcApi.cpp
        src/api/ApiUtils.h
        src/api/RefreshTokenInterceptor.h
        src/api/RefreshTokenInterceptor.cpp
        DEPS
        juce::juce_core
        juce::juce_events
        account
        lager
        cpr::cpr)

zones_add_test(MODULE api SOURCES test/TestDeviceApi.cpp test/TestOidcApi.cpp DEPS api)
