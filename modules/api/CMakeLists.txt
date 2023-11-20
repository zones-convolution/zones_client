zones_add_module(
        MODULE
        api
        SOURCES
        src/api/ApiRequest.cpp
        src/api/ApiRequest.h
        src/api/ApiRequestJob.h
        src/api/ApiRequestJob.cpp
        src/api/ApiRequestService.h
        src/api/ApiRequestService.cpp
        src/api/AuthenticatedApiRequestService.h
        src/api/AuthenticatedApiRequestService.cpp
        src/api/DeviceApi.h
        src/api/DeviceApi.cpp
        src/api/OidcApi.h
        src/api/OidcApi.cpp
        src/api/RefreshTokenJob.h
        src/api/RefreshTokenJob.cpp
        DEPS
        juce::juce_core
        juce::juce_events
        account
        lager)

zones_add_test(MODULE api SOURCES test/TestApiRequest.cpp test/TestDeviceApi.cpp DEPS api)
