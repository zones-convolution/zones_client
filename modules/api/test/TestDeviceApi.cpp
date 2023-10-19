#include "api/DeviceApi.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("device api tests", "[DeviceApi]")
{
    SECTION ("requests device code")
    {
        juce::WaitableEvent request_finished_event;

        ApiRequestService api_request_service;
        DeviceApi::DeviceCodeRequest (api_request_service,
                                      {.success =
                                           [&] (const DeviceApi::DeviceCodeSuccessResponse & res)
                                       {
                                           REQUIRE (res.device_code.isNotEmpty ());
                                           REQUIRE (res.user_code.isNotEmpty ());
                                           REQUIRE (res.verification_uri.isNotEmpty ());
                                           REQUIRE (res.verification_uri_complete.isNotEmpty ());
                                           REQUIRE (res.expires_in != 0);
                                           REQUIRE (res.interval != 0);

                                           request_finished_event.signal ();
                                       },
                                       .fail = [] (auto res) {},
                                       .progress = [] (auto progress) {},
                                       .start = [] {}});

        auto did_signal = request_finished_event.wait (4000);
        REQUIRE (did_signal);
    }
}