#include <vector>

#include "serialization/b64.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::serialization;
using namespace std::string_literals;

TEST_CASE("round trip") {
    auto s = "Many hands make light work.";
    auto in = std::vector<unsigned char>(s, s+strlen(s));
    auto encoded = base64_encode(in);
    auto out = base64_decode(encoded);
    auto expected = std::string("TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu");
    
    REQUIRE(expected == encoded);
    REQUIRE(in == out);
}
