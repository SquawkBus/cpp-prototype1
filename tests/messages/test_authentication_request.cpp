#include <memory>
#include <vector>

#include "messages/messages.hpp"
#include "serialization/basic_token.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using squawkbus::messages::Message;
using squawkbus::messages::AuthenticationRequest;
using squawkbus::serialization::encode_basic_token;

using namespace std::string_literals;

TEST_CASE("method none")
{
    std::shared_ptr<Message> m0 = std::make_shared<AuthenticationRequest>(
        "none",
        ""
    );
    
    auto frame = m0->serialize();

    auto m1 = Message::deserialize(frame);

    REQUIRE(m0 == m1);
}

TEST_CASE("method basic")
{
    auto token = encode_basic_token("aladdin", "opensesame");
    std::shared_ptr<Message> m0 = std::make_shared<AuthenticationRequest>(
        "basic",
        token
    );
    
    auto frame = m0->serialize();

    auto m1 = Message::deserialize(frame);

    REQUIRE(m0 == m1);
}
