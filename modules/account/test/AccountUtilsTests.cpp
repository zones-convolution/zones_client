#include "account/AccountUtils.h"

#include <catch2/catch_test_macros.hpp>

SCENARIO ("reading id tokens", "[AccountUtils]")
{
    GIVEN ("an invalid token")
    {
        REQUIRE_THROWS (ReadProfileFromIdToken ("invalid_token"));
    }

    GIVEN ("a valid JWT with incorrect claims")
    {
        auto valid_jwt_with_incorrect_claims =
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE2OTc5MDE3MDl9.vPU4WDeOIKajiOHZtC08ZRwI8tPKITKnPzyf-QYMeVo";
        REQUIRE_THROWS (ReadProfileFromIdToken (valid_jwt_with_incorrect_claims));
    }

    GIVEN ("a valid JWT with correct claims")
    {
        auto valid_jwt_with_correct_claims =
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlbWFpbCI6ImVtYWlsQGRvbWFpbi50bGQiLCJjb2duaXRvOnVzZXJuYW1lIjoidXNlcm5hbWUiLCJjb2duaXRvOmdyb3VwcyI6WyJncm91cF8xIiwiZ3JvdXBfMiJdLCJpYXQiOjE2OTc5MDIxNjN9.gI6v1EoufC6cPqGh8BqzoKpWN4RYzXCvY6iey78YGe0";
        auto profile = ReadProfileFromIdToken (valid_jwt_with_correct_claims);
        REQUIRE (profile.email == "email@domain.tld");
        REQUIRE (profile.username == "username");
        REQUIRE (profile.groups.size () == 2);
        REQUIRE (profile.groups [0] == "group_1");
        REQUIRE (profile.groups [1] == "group_2");
    }
}