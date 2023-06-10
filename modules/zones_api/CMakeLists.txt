zones_add_module(
        MODULE zones_api
        SOURCES src/zones_api/ApiRequest.cpp src/zones_api/ApiRequest.h src/zones_api/ApiRequestJob.h src/zones_api/ApiRequestJob.cpp src/zones_api/ApiRequestService.h src/zones_api/ApiRequestService.cpp
        DEPS juce::juce_core juce::juce_events
)

zones_add_test(
        MODULE zones_api
        SOURCES test/TestApiRequest.cpp
        DEPS zones_api
)