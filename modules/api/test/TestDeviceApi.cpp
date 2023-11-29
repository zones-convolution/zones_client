#include "api/DeviceApi.h"

#include <catch2/catch_test_macros.hpp>
#include <cpr/cpr.h>
#include <juce_core/juce_core.h>

TEST_CASE ("device api tests", "[DeviceApi]")
{
    const auto base_url = "https://oauth2.googleapis.com";
    SECTION ("requests device code")
    {
        auto request = DeviceApi::DeviceCodeRequest (
            base_url,
            cpr::Payload {
                {"client_id",
                 "687494975680-4m8b1mala896lep7b5hghlpigo1bsjvf.apps.googleusercontent.com"},
                {"scope", "email profile"}});
        auto response = request.get ();
        REQUIRE (! response.error);
        REQUIRE (response.status_code == 200);
        auto device_code_success = DeviceApi::ReadDeviceCodeSuccess (response);
        REQUIRE (device_code_success.device_code.isNotEmpty ());
        std::cout << device_code_success.device_code << std::endl;
    }
}
