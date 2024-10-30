#include "io/endpoint.hpp"

#include <array>
#include <cstdint>
#include <format>
#include <string>

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::io;

TEST_CASE("roundtrip")
{
    using namespace std::string_literals;

    auto endpoint_in = "0.0.0.0:8080"s;
    auto endpoint = Endpoint::parse(endpoint_in);
    auto endpoint_out = std::string(endpoint);

    REQUIRE(endpoint_in == endpoint_out);
}

TEST_CASE("empty")
{
    using namespace std::string_literals;

    auto endpoint = Endpoint();

    REQUIRE(endpoint.empty());
}

TEST_CASE("parameter ctor")
{
    using namespace std::string_literals;

    auto host = "beast.jetblack.net"s;
    auto port = std::uint16_t { 8080 };
    auto endpoint = Endpoint(host, port);

    REQUIRE_FALSE(endpoint.empty());
    REQUIRE(endpoint.host() == host);
    REQUIRE(endpoint.port() == port);
    REQUIRE(std::string(endpoint) == std::format("{}:{}", host, port));
}
