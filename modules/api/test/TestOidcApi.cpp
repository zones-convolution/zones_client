#include "api/OidcApi.h"

#include <catch2/catch_test_macros.hpp>
#include <cpr/cpr.h>
#include <juce_core/juce_core.h>

TEST_CASE ("oidc api tests", "[OidcApi]")
{
    SECTION ("discovery")
    {
        const auto base_url = "https://cognito-idp.eu-west-2.amazonaws.com/eu-west-2_rzvTaRw3J";
        auto request = OidcApi::DiscoverRequest (base_url);
        request.Cancel ();
        auto response = request.get ();
        REQUIRE (! response.error);
        REQUIRE (response.status_code == 200);
        auto discover_success = OidcApi::ReadDiscoverSuccess (response);
        REQUIRE (discover_success.token_endpoint.isNotEmpty ());
        std::cout << discover_success.token_endpoint << std::endl;
    }
}
