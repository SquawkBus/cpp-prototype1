#include <vector>

#include "serialization/basic_token.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::serialization;
using namespace std::string_literals;

TEST_CASE("basic token")
{
    auto username = "aladdin"s;
    auto password = "opensesame";
    auto expected = std::string("YWxhZGRpbjpvcGVuc2VzYW1l");

    auto token = encode_basic_token(username, password);
    REQUIRE(token == expected);

    auto [username_out, password_out] = decode_basic_token(token);
    REQUIRE(username == username_out);
    REQUIRE(password == password_out);
}