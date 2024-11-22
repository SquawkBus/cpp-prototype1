#include <vector>

#include "serialization/b64.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::serialization;

TEST_CASE("round trip") {
    auto s = "Many hands make light work.";
    auto in = std::vector<unsigned char>(s, s+strlen(s));
    auto encoded = base64_encode(in);
    auto out = base64_decode(encoded);
    auto expected = std::string("TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu");
    
    REQUIRE(expected == encoded);
    REQUIRE(in == out);
}

TEST_CASE("basic auth")
{
    auto s = "aladdin:opensesame";
    auto in = std::vector<unsigned char>(s, s+strlen(s));
    auto encoded = base64_encode(in);
    auto out = base64_decode(encoded);
    auto expected = std::string("YWxhZGRpbjpvcGVuc2VzYW1l");
    
    REQUIRE(expected == encoded);
    REQUIRE(in == out);
}