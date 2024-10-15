#define CATCH_CONFIG_MAIN

#include <array>

#include "serialization/network_bytes.hpp"

#include "catch2/catch.hpp"

using namespace squawkbus::serialization;

TEST_CASE("roundtrip") {
    char array_in[4], array_out[4];

    uint32_t value_in, value_out;

    value_in = 0;
    network_long_to_buf(value_in, array_out);
    value_out = buf_to_network_long(array_out);
    REQUIRE(value_in == value_out);

    value_in = 1;
    network_long_to_buf(value_in, array_out);
    value_out = buf_to_network_long(array_out);
    REQUIRE(value_in == value_out);

    value_in = 9;
    network_long_to_buf(value_in, array_out);
    value_out = buf_to_network_long(array_out);
    REQUIRE(value_in == value_out);

    value_in = 65000;
    network_long_to_buf(value_in, array_out);
    value_out = buf_to_network_long(array_out);
    REQUIRE(value_in == value_out);
}