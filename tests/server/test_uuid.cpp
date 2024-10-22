#include <array>

#include "server/uuid.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::server;

TEST_CASE("roundtrip") {

    auto u1 = uuid::generate();
    auto s1 = u1.str();
    auto u2 = uuid::parse(s1);

    REQUIRE(u1 == u2);
}
