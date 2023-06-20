#include "ir_repository/io/IrReader.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE ("ir_reader can read from disk", "[IrReader]")
{
    SECTION ("resizing bigger changes size and capacity")
    {
        v.resize (10);

        REQUIRE (v.size () == 10);
        REQUIRE (v.capacity () >= 10);
    }
}